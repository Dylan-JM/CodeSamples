// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/PlayerCharacter.h"
#include "BlueprintGameplayTagLibrary.h"
#include "GlobalEnums.h"
#include "HDGameplayTags.h"
#include "NiagaraComponent.h"
#include "Abilities/HDAbilitySystemLibrary.h"
#include "Abilities/HDAttributeSet.h"
#include "Abilities/Data/AbilityInfo.h"
#include "Abilities/Data/LevelUpInfo.h"
#include "Abilities/Debuffs/DebuffNiagaraComponent.h"
#include "Actor/MagicCircle.h"
#include "Actor/Shield.h"
#include "Actor/EquippableActors/Armor.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/HDAbilitySystemComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/LightingComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Game/HDGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "HorrorDungeon/HorrorDungeon.h"
#include "HUD/BaseHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerController/BasePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter/HealthCharacter.h"
#include "Weapons/BaseWeapon.h"


APlayerCharacter::APlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->SetAutoActivate(false);

	InsuranceInventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InsuranceInventoryComponent");
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f,540.f,0.f);

	CharacterClass = ECharacterClass::Elementalist;

}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//UpdateLightVignette();
	InterpFOV(DeltaSeconds);
	GetTraceResult(DefaultTraceDistance);
	
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, MovementDirectionVector);
	DOREPLIFETIME(APlayerCharacter, ImpactPoint);
	DOREPLIFETIME(APlayerCharacter, bIsStepping);
	DOREPLIFETIME(APlayerCharacter, bIsSprinting);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacterController = Cast<ABasePlayerController>(GetController());
	DefaultFOV = GetFollowCamera()->FieldOfView;
	CurrentFOV = DefaultFOV;
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	HealthCharacter = Cast<AHealthCharacter>(UGameplayStatics::GetActorOfClass(this, AHealthCharacter::StaticClass()));
	if (HealthCharacter)
	{
		HealthCharacter->PlayerAdded(this);
	}
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (PlayerCharacterController == nullptr) PlayerCharacterController = Cast<ABasePlayerController>(NewController);
	
	// Init ability actor info for the Server
	InitAbilityActorInfo();

	 //Add back in when just testing
	InitializeDefaultAttributes();
	AddCharacterAbilities();
	
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
	
}

void APlayerCharacter::InitAbilityActorInfo()
{
	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (!IsValid(PS)) return;
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	Cast<UHDAbilitySystemComponent>(PS->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AttributeSetBase = PS->GetAttributeSetBase();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FHDGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::StunTagChanged);
	
	if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetController()))
	{
		if (ABaseHUD* BaseHUD = Cast<ABaseHUD>(PlayerController->GetHUD()))
		{
			BaseHUD->InitOverlay(PlayerController, PS, AbilitySystemComponent, AttributeSetBase);
		}
	}
	
	UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(AttributeSetBase);
	BaseArmor = HDAS->GetArmor();
}

void APlayerCharacter::InterpFOV(float DeltaTime)
{
	if (EquippedWeapon1 == nullptr) return;
	if (bIsAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, ZoomedFOV, DeltaTime, ZoomInterpSpeed);
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, ZoomInterpSpeed);
	}
	GetFollowCamera()->SetFieldOfView(CurrentFOV);
}

void APlayerCharacter::Die(const FVector& DeathImpulse, bool bRagdoll)
{
	Super::Die(DeathImpulse, false);

	if (!HealthCharacter->bCanRevive)
	{
		EmptyInventory();
		ClientSaveProgress();
	}
}

void APlayerCharacter::EmptyInventory_Implementation()
{
	//InventoryComponent->Inventory.Empty();
	EquippedWeapon1 = nullptr;
	EquippedOffhandItem = nullptr;
	Helmet = nullptr;
	Chestplate = nullptr;
	Gauntlets = nullptr;
	Legs = nullptr;
	EquippedAmulet1 = nullptr;
	EquippedAmulet2 = nullptr;
	EquippedAmulet3 = nullptr;
}

void APlayerCharacter::SpawnItemFromActor(TSubclassOf<AEquippableActor> ActorToSpawn, bool GroundClamp, int32 InStacks)
{
	FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 150.f);
	if (GroundClamp)
	{
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation, (SpawnLocation - FVector(0,0,1000)), ECC_Visibility))
		{
			SpawnLocation = HitResult.Location;
		}
	}
	AEquippableActor* SpawnedItem = GetWorld()->SpawnActorDeferred<AEquippableActor>(ActorToSpawn, FTransform(SpawnLocation));
	SpawnedItem->SetReplicates(false);
	SpawnedItem->CurrentStacks = InStacks;
	SpawnedItem->FinishSpawning(FTransform(SpawnLocation));
}

void APlayerCharacter::SetAiming(bool bInIsAiming)
{
	if (bIsAiming == bInIsAiming) return;
	if (PlayerCharacterController == nullptr) return;
	bIsAiming = bInIsAiming;
	SetSprinting(false);
	ChangeMovementSpeed(bIsAiming);
	ServerSetAiming(bInIsAiming);
	if (bIsAiming)
	{
		if (PlayerCharacterController->CrosshairWidget)
		{
			PlayerCharacterController->CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
		}
		
	}
	else if (!bIsAiming)
	{
		if (PlayerCharacterController->CrosshairWidget) PlayerCharacterController->CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void APlayerCharacter::ServerSetAiming_Implementation(bool bInIsAiming)
{
	bIsAiming = bInIsAiming;
	SetSprinting(false);
	ChangeMovementSpeed(bIsAiming);
}

void APlayerCharacter::SetSprinting(bool bInIsSprinting)
{
	if (bIsSprinting == bInIsSprinting || bIsAiming) return;
	bIsSprinting = bInIsSprinting;
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else if (!bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
	ServerSetSprinting(bInIsSprinting);
}

void APlayerCharacter::ServerSetSprinting_Implementation(bool bInIsSprinting)
{
	bIsSprinting = bInIsSprinting;
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else if (!bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}

void APlayerCharacter::ClientSaveProgress()
{

	if (PlayerSaveGame == nullptr) return;
	
	if (ABasePlayerState* BasePlayerState = Cast<ABasePlayerState>(GetPlayerState()))
	{
		PlayerSaveGame->PlayerLevel = BasePlayerState->GetPlayerLevel();
		PlayerSaveGame->XP = BasePlayerState->GetXP();
		PlayerSaveGame->AttributePoints = BasePlayerState->GetAttributePoints();
		PlayerSaveGame->SpellPoints = BasePlayerState->GetSpellPoints();
	}
	
	PlayerSaveGame->Strength = UHDAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
	PlayerSaveGame->Intelligence = UHDAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
	PlayerSaveGame->Resilience = UHDAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
	PlayerSaveGame->Vigor = UHDAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
	PlayerSaveGame->CharacterClass = CharacterClass;

	PlayerSaveGame->bFirstTimeLoadIn = false;
	
	UHDAbilitySystemComponent* HDASC = Cast<UHDAbilitySystemComponent>(AbilitySystemComponent);
	FForEachAbility SaveAbilityDelegate;
	PlayerSaveGame->SavedAbilities.Empty();
	SaveAbilityDelegate.BindLambda([this, HDASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		const FGameplayTag AbilityTag = HDASC->GetAbilityTagFromSpec(AbilitySpec);
		FHDAbilityInfo Info = PlayerAbilityInfo->FindAbilityInfoForTag(AbilityTag);
			
		FSavedAbility SavedAbility;
		SavedAbility.GameplayAbility = Info.Ability;
		SavedAbility.AbilityLevel = AbilitySpec.Level;
		SavedAbility.AbilitySlot = HDASC->GetSlotFromAbilityTag(AbilityTag);
		SavedAbility.AbilityStatus = HDASC->GetStatusFromAbilityTag(AbilityTag);
		SavedAbility.AbilityTag = AbilityTag;
		SavedAbility.AbilityType = Info.AbilityType;

		PlayerSaveGame->SavedAbilities.AddUnique(SavedAbility);
	});
	
	HDASC->ForEachAbility(SaveAbilityDelegate);
	
	PlayerSaveGame->ClearEquippedItems();
	
	if (EquippedWeapon1) PlayerSaveGame->EquippedWeapon1 = EquippedWeapon1.GetClass();
	if (EquippedOffhandItem) PlayerSaveGame->EquippedOffHandItem = EquippedOffhandItem.GetClass();
	if (Helmet) PlayerSaveGame->EquippedHelmet = Helmet.GetClass();
	if (Chestplate) PlayerSaveGame->EquippedChestplate = Chestplate.GetClass();
	if (Gauntlets) PlayerSaveGame->EquippedGaunlets = Gauntlets.GetClass();
	if (Legs) PlayerSaveGame->EquippedLegs = Legs.GetClass();
	if (EquippedAmulet1) PlayerSaveGame->EquippedAmulet1 = EquippedAmulet1.GetClass();
	if (EquippedAmulet2) PlayerSaveGame->EquippedAmulet2 = EquippedAmulet2.GetClass();
	if (EquippedAmulet3) PlayerSaveGame->EquippedAmulet3 = EquippedAmulet3.GetClass();

	
	PlayerSaveGame->SavedInventory.Empty();

	// Save values in normal inventory
	
	for (const auto Item : InventoryComponent->Inventory)
	{
		FSavedItems SavedItem;
		// Access the item's values
		SavedItem.ItemValues = Item.Key;
		SavedItem.Item = Item.Value;
			
		// Do something with the item's values
		PlayerSaveGame->SavedInventory.Emplace(SavedItem);
	}
	
	// Save values in insurance inventory

	for (const auto Item : InsuranceInventoryComponent->Inventory)
	{
		FSavedItems SavedItem;
		// Access the item's values
		SavedItem.ItemValues = Item.Key;
		SavedItem.Item = Item.Value;
			
		// Do something with the item's values
		PlayerSaveGame->SavedInsuranceInventory.Emplace(SavedItem);
	}
	
	SaveInGameProgressData();
}

void APlayerCharacter::SaveInGameProgressData_Implementation()
{
	UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = HDGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = HDGameInstance->LoadSlotIndex;

	UGameplayStatics::SaveGameToSlot(PlayerSaveGame, InGameLoadSlotName, InGameLoadSlotIndex);
}

void APlayerCharacter::ClientLoadProgress_Implementation()
{

	UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = HDGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = HDGameInstance->LoadSlotIndex;
	
	PlayerSaveGame = GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
	
	if (PlayerSaveGame->bFirstTimeLoadIn)
	{
		ServerFirstTimeLoadIn();
		EquipStartingItems();
	}
	else
	{
		ServerLoadProgress(PlayerSaveGame->PlayerLevel, PlayerSaveGame->XP, PlayerSaveGame->AttributePoints, PlayerSaveGame->SpellPoints);
		ServerLoadDefaultAttributes(PlayerSaveGame->Strength, PlayerSaveGame->Intelligence, PlayerSaveGame->Resilience, PlayerSaveGame->Vigor);
		ServerAddCharacterAbilitiesFromClientSaveData(PlayerSaveGame->SavedAbilities);
		
		AddSavedItemsToInventory();
	}
	
}

void APlayerCharacter::ServerLoadDefaultAttributes_Implementation(float InStrength, float InInt, float InRes,
	float InVigor)
{
	UHDAbilitySystemLibrary::InitializeDefaultAttributesClientFromSaveData(this, AbilitySystemComponent, InStrength, InInt, InRes, InVigor);
}

void APlayerCharacter::ServerLoadProgress_Implementation(int32 InLevel, int32 InXP, int32 InAttributePoints, int32 InSpellPoints)
{
	if (ABasePlayerState* BasePlayerState = Cast<ABasePlayerState>(GetPlayerState()))
	{
		BasePlayerState->SetLevel(InLevel);
		BasePlayerState->SetXP(InXP);
		BasePlayerState->SetToAttributePoints(InAttributePoints);
		BasePlayerState->SetToSpellPoints(InSpellPoints);
	}
}

void APlayerCharacter::ServerFirstTimeLoadIn_Implementation()
{
	InitializeDefaultAttributes();
	AddCharacterAbilities();
}

void APlayerCharacter::ServerAddCharacterAbilitiesFromClientSaveData_Implementation(const TArray<FSavedAbility>& InSavedAbilities)
{
	if (UHDAbilitySystemComponent* HDASC = Cast<UHDAbilitySystemComponent>(AbilitySystemComponent))
	{
		HDASC->AddCharacterAbilitiesFromClientSaveData(InSavedAbilities);
		if (StaminaRegeneration)
		{
			ApplyEffectToSelf(StaminaRegeneration, 1.f);
		}
	}
}

ULoadScreenSaveGame* APlayerCharacter::GetSaveSlotData(const FString& SlotName, int32 SlotIndex)
{
	USaveGame* SaveGameObject;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void APlayerCharacter::Jump()
{
	Super::Jump();
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

void APlayerCharacter::TraceUnderCrosshairs(FHitResult& TraceHitResult, float TraceDistance, bool bUseVisibilityChannel)
{
	if (!IsValid(PlayerCharacterController)) return;
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	
	ECollisionChannel TraceChannel;
	if (bUseVisibilityChannel)
	{
		TraceChannel = ECC_Visibility;
	}
	else
	{
		TraceChannel = IsValid(PlayerCharacterController->MagicCircle) ? ECC_ExcludePawns : ECC_Target;
	}
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);
	
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		float DistanceToCharacter = (GetActorLocation() - Start).Size();
		Start += CrosshairWorldDirection * (DistanceToCharacter + 100.f);
		
		// This is for if I want the thing to fire straight forward FVector EndLocation = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, CrosshairWorldDirection.Z);
		FVector End = Start + CrosshairWorldDirection * TraceDistance;
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, TraceChannel);
		if (!TraceHitResult.bBlockingHit)
		{
			TraceHitResult.ImpactPoint = End;
		}
	}

	LastActor = ThisActor;
	ThisActor = Cast<IHighlightInterface>(TraceHitResult.GetActor());
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor)
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}

UObject* APlayerCharacter::GetThisActor() const
{
	if (ThisActor)
	{
		return ThisActor->_getUObject();
	}
	else
	{
		return nullptr;
	}
}

void APlayerCharacter::EquipItem(TSubclassOf<AEquippableActor> ItemToEquip, EItemType ItemType, int32 SlotIndex)
{
	UHDAbilitySystemComponent* HDASC = Cast<UHDAbilitySystemComponent>(GetAbilitySystemComponent());
	UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(AttributeSetBase);
	const USkeletalMeshSocket* ItemSocket = GetMesh()->GetSocketByName(ItemToEquip.GetDefaultObject()->DefaultSocketName);
	AArmor* Armor = Cast<AArmor>(ItemToEquip.GetDefaultObject());
	switch (ItemType)
	{
	case EItemType::EIT_Helmet:
		HelmetMesh->SetSkeletalMesh(Armor->ArmorMesh);
		TotalArmor += Armor->ArmorIncrease;
		HDAS->SetArmor(TotalArmor);
		Helmet = Armor;
		break;
	case EItemType::EIT_Chest:
		ChestplateMesh->SetSkeletalMesh(Armor->ArmorMesh);
		TotalArmor += Armor->ArmorIncrease;
		HDAS->SetArmor(TotalArmor);
		Chestplate = Armor;
		break;
	case EItemType::EIT_Arms:
		GauntletsMesh->SetSkeletalMesh(Armor->ArmorMesh);
		TotalArmor += Armor->ArmorIncrease;
		HDAS->SetArmor(TotalArmor);
		Gauntlets = Armor;
		break;
	case EItemType::EIT_Legs:
		LegsMesh->SetSkeletalMesh(Armor->ArmorMesh);
		TotalArmor += Armor->ArmorIncrease;
		HDAS->SetArmor(TotalArmor);
		Legs = Armor;
		break;
	case EItemType::EIT_Weapon:
		switch (SlotIndex)
		{
		case 1:
			EquippedWeapon1 = GetWorld()->SpawnActorDeferred<ABaseWeapon>(ItemToEquip,FTransform(), this);
			if (EquippedWeapon1)
			{
				EquippedWeapon1->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedWeapon1, GetMesh());
				EquippedWeapon1->SetOwner(this);
				EquippedWeapon1->bOnGround = false;
				EquippedWeapon1->SetReplicates(true);
				EquippedWeapon1->FinishSpawning(FTransform());
				SetCombatStyle(EquippedWeapon1->CombatStyle);

				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(EquippedWeapon1->DefaultAbility, 1);
				for (auto ActiveAbility : HDASC->GetActivatableAbilities())
				{
					//GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Yellow, ActiveAbility.GetDebugString());
					if (ActiveAbility.Ability->AbilityTags == AbilitySpec.Ability->AbilityTags)
					{
						return;
					}
				}
				HDASC->AddCharacterAbility(EquippedWeapon1->DefaultAbility);
			}
			break;
		case 2:
			EquippedOffhandItem = GetWorld()->SpawnActorDeferred<AEquippableActor>(ItemToEquip, FTransform(), this);
			if (EquippedOffhandItem)
			{
				ItemSocket = GetMesh()->GetSocketByName(ItemToEquip.GetDefaultObject()->AltSocketName);
				EquippedOffhandItem->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedOffhandItem, GetMesh());
				EquippedOffhandItem->SetOwner(this);
				EquippedOffhandItem->bOnGround = false;
				EquippedOffhandItem->SetReplicates(true);
				EquippedOffhandItem->FinishSpawning(FTransform());
				if (EquippedOffhandItem.IsA(ABaseWeapon::StaticClass()) && !IsValid(EquippedWeapon1))
				{
					SetCombatStyle(EquippedOffhandItem->CombatStyle);
				}
			}
			break;
		default: ;
		}
	case EItemType::EIT_Consumable:
		break;
	case EItemType::EIT_Amulet:
		switch (SlotIndex)
		{
		case 1:
			EquippedAmulet1 = GetWorld()->SpawnActorDeferred<AEquippableActor>(ItemToEquip, FTransform(), this);
			if (EquippedAmulet1)
			{
				ItemSocket = GetMesh()->GetSocketByName(FName("Amulet1"));
				EquippedAmulet1->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedAmulet1, GetMesh());
				EquippedAmulet1->SetOwner(this);
				EquippedAmulet1->bOnGround = false;
				EquippedAmulet1->SetReplicates(true);
				EquippedAmulet1->FinishSpawning(FTransform());
			}
			break;
			
		case 2:
			EquippedAmulet2 = GetWorld()->SpawnActorDeferred<AEquippableActor>(ItemToEquip, FTransform(), this);
			if (EquippedAmulet2)
			{
				ItemSocket = GetMesh()->GetSocketByName(FName("Amulet2"));
				EquippedAmulet2->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedAmulet2, GetMesh());
				EquippedAmulet2->SetOwner(this);
				EquippedAmulet2->bOnGround = false;
				EquippedAmulet2->SetReplicates(true);
				EquippedAmulet2->FinishSpawning(FTransform());
			}
			break;
			
		case 3:
			EquippedAmulet3 = GetWorld()->SpawnActorDeferred<AEquippableActor>(ItemToEquip, FTransform(), this);
			if (EquippedAmulet3)
			{
				ItemSocket = GetMesh()->GetSocketByName(FName("Amulet3"));
				EquippedAmulet3->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedAmulet3, GetMesh());
				EquippedAmulet3->SetOwner(this);
				EquippedAmulet3->bOnGround = false;
				EquippedAmulet3->SetReplicates(true);
				EquippedAmulet3->FinishSpawning(FTransform());
			}
			break;
		default: ;
		}
		
	default: ;
	}
	if (!HasAuthority()) ServerEquipItem(ItemToEquip, ItemType, SlotIndex);
}

void APlayerCharacter::ServerEquipItem_Implementation(TSubclassOf<AEquippableActor> ItemToEquip, EItemType ItemType, int32 SlotIndex)
{
	UHDAbilitySystemComponent* HDASC = Cast<UHDAbilitySystemComponent>(GetAbilitySystemComponent());
	UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(AttributeSetBase);
	const USkeletalMeshSocket* ItemSocket = GetMesh()->GetSocketByName(ItemToEquip.GetDefaultObject()->DefaultSocketName);
	AArmor* Armor = Cast<AArmor>(ItemToEquip.GetDefaultObject());
	switch (ItemType)
	{
	case EItemType::EIT_Helmet:
		HelmetMesh->SetSkeletalMesh(Armor->ArmorMesh);
		TotalArmor += Armor->ArmorIncrease;
		HDAS->SetArmor(TotalArmor);
		Helmet = Armor;
		break;
	case EItemType::EIT_Chest:
		ChestplateMesh->SetSkeletalMesh(Armor->ArmorMesh);
		TotalArmor += Armor->ArmorIncrease;
		HDAS->SetArmor(TotalArmor);
		Chestplate = Armor;
		break;
	case EItemType::EIT_Arms:
		GauntletsMesh->SetSkeletalMesh(Armor->ArmorMesh);
		TotalArmor += Armor->ArmorIncrease;
		HDAS->SetArmor(TotalArmor);
		Gauntlets = Armor;
		break;
	case EItemType::EIT_Legs:
		LegsMesh->SetSkeletalMesh(Armor->ArmorMesh);
		TotalArmor += Armor->ArmorIncrease;
		HDAS->SetArmor(TotalArmor);
		Legs = Armor;
		break;
	case EItemType::EIT_Weapon:
		switch (SlotIndex)
		{
	case 1:
		EquippedWeapon1 = GetWorld()->SpawnActorDeferred<ABaseWeapon>(ItemToEquip,FTransform(), this);
			if (EquippedWeapon1)
			{
				EquippedWeapon1->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedWeapon1, GetMesh());
				EquippedWeapon1->SetOwner(this);
				EquippedWeapon1->bOnGround = false;
				EquippedWeapon1->SetReplicates(true);
				EquippedWeapon1->FinishSpawning(FTransform());
				SetCombatStyle(EquippedWeapon1->CombatStyle);

				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(EquippedWeapon1->DefaultAbility, 1);
				for (auto ActiveAbility : HDASC->GetActivatableAbilities())
				{
					if (ActiveAbility.Ability->AbilityTags == AbilitySpec.Ability->AbilityTags)
					{
						return;
					}
				}
				HDASC->AddCharacterAbility(EquippedWeapon1->DefaultAbility);
			}
			break;
	case 2:
		EquippedOffhandItem = GetWorld()->SpawnActorDeferred<AEquippableActor>(ItemToEquip, FTransform(), this);
			if (EquippedOffhandItem)
			{
				ItemSocket = GetMesh()->GetSocketByName(ItemToEquip.GetDefaultObject()->AltSocketName);
				EquippedOffhandItem->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedOffhandItem, GetMesh());
				EquippedOffhandItem->SetOwner(this);
				EquippedOffhandItem->bOnGround = false;
				EquippedOffhandItem->SetReplicates(true);
				EquippedOffhandItem->FinishSpawning(FTransform());
				if (EquippedOffhandItem.IsA(ABaseWeapon::StaticClass()) && !IsValid(EquippedWeapon1))
				{
					SetCombatStyle(EquippedOffhandItem->CombatStyle);
				}
			}
			break;
	default: ;
		}
	case EItemType::EIT_Consumable:
		break;
	case EItemType::EIT_Amulet:
		switch (SlotIndex)
		{
		case 1:
			EquippedAmulet1 = GetWorld()->SpawnActorDeferred<AEquippableActor>(ItemToEquip, FTransform(), this);
			if (EquippedAmulet1)
			{
				ItemSocket = GetMesh()->GetSocketByName(FName("Amulet1"));
				EquippedAmulet1->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedAmulet1, GetMesh());
				EquippedAmulet1->SetOwner(this);
				EquippedAmulet1->bOnGround = false;
				EquippedAmulet1->SetReplicates(true);
				EquippedAmulet1->FinishSpawning(FTransform());
			}
			break;
			
		case 2:
			EquippedAmulet2 = GetWorld()->SpawnActorDeferred<AEquippableActor>(ItemToEquip, FTransform(), this);
			if (EquippedAmulet2)
			{
				ItemSocket = GetMesh()->GetSocketByName(FName("Amulet2"));
				EquippedAmulet2->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedAmulet2, GetMesh());
				EquippedAmulet2->SetOwner(this);
				EquippedAmulet2->bOnGround = false;
				EquippedAmulet2->SetReplicates(true);
				EquippedAmulet2->FinishSpawning(FTransform());
			}
			break;
		case 3:
			EquippedAmulet3 = GetWorld()->SpawnActorDeferred<AEquippableActor>(ItemToEquip, FTransform(), this);
			if (EquippedAmulet3)
			{
				ItemSocket = GetMesh()->GetSocketByName(FName("Amulet3"));
				EquippedAmulet3->GetItemMesh()->SetSimulatePhysics(false);
				ItemSocket->AttachActor(EquippedAmulet3, GetMesh());
				EquippedAmulet3->SetOwner(this);
				EquippedAmulet3->bOnGround = false;
				EquippedAmulet3->SetReplicates(true);
				EquippedAmulet3->FinishSpawning(FTransform());
			}
			break;
		default: ;
		}

	default: ;
	}
}

void APlayerCharacter::UnequipItem(EItemType ItemType, int32 SlotIndex)
{
	UHDAbilitySystemComponent* HDASC = Cast<UHDAbilitySystemComponent>(GetAbilitySystemComponent());
	UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(AttributeSetBase);
	switch (ItemType)
	{
	case EItemType::EIT_Helmet:
		if (!IsValid(Helmet)) break;
		HelmetMesh->SetSkeletalMesh(nullptr);
		TotalArmor -= Helmet->ArmorIncrease;
		Helmet = nullptr;
		HDAS->SetArmor(TotalArmor);
		break;
	case EItemType::EIT_Chest:
		if (!IsValid(Chestplate)) break;
		ChestplateMesh->SetSkeletalMesh(nullptr);
		TotalArmor -= Chestplate->ArmorIncrease;
		Chestplate = nullptr;
		HDAS->SetArmor(TotalArmor);
		break;
	case EItemType::EIT_Arms:
		if (!IsValid(Gauntlets)) break;
		GauntletsMesh->SetSkeletalMesh(nullptr);
		TotalArmor -= Gauntlets->ArmorIncrease;
		Gauntlets = nullptr;
		HDAS->SetArmor(TotalArmor);
		break;
	case EItemType::EIT_Legs:
		if (!IsValid(Legs)) break;
		LegsMesh->SetSkeletalMesh(nullptr);
		TotalArmor -= Legs->ArmorIncrease;
		Legs = nullptr;
		HDAS->SetArmor(TotalArmor);
		break;
	case EItemType::EIT_Weapon:
		switch (SlotIndex)
		{
		case 1:
			if (EquippedWeapon1)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(EquippedWeapon1->DefaultAbility, 1);
				for (auto ActiveAbility : HDASC->GetActivatableAbilities())
				{
					if (ActiveAbility.Ability->AbilityTags == AbilitySpec.Ability->AbilityTags)
					{
						HDASC->ClearAbility(ActiveAbility.Handle);
					}
				}
				EquippedWeapon1->Destroy();
				EquippedWeapon1 = nullptr;
				if (!IsValid(EquippedOffhandItem) || !EquippedOffhandItem.IsA(ABaseWeapon::StaticClass()))
				{
					SetCombatStyle(ECombatStyle::ECS_Unarmed);
				}
			}
			break;
		case 2:
			if (EquippedOffhandItem)
			{
				EquippedOffhandItem->Destroy();
				EquippedOffhandItem = nullptr;
				if (!IsValid(EquippedWeapon1) || !EquippedWeapon1.IsA(ABaseWeapon::StaticClass()))
				{
					SetCombatStyle(ECombatStyle::ECS_Unarmed);
				}
			}
			break;
		default: ;
		}
		break;
	case EItemType::EIT_Consumable:
		break;
	case EItemType::EIT_Amulet:
		switch (SlotIndex)
		{
		case 1:
			if (EquippedAmulet1)
			{
				EquippedAmulet1->Destroy();
				EquippedAmulet1 = nullptr;
			}
			break;
		case 2:
			if (EquippedAmulet2)
			{
				EquippedAmulet2->Destroy();
				EquippedAmulet2 = nullptr;
			}
			break;
		case 3:
			if (EquippedAmulet3)
			{
				EquippedAmulet3->Destroy();
				EquippedAmulet3 = nullptr;
			}
			break;
		default: ;
		}
		break;
		
	default: ;
	}
	if (!HasAuthority()) ServerUnequipItem(ItemType, SlotIndex);
}

void APlayerCharacter::ServerUnequipItem_Implementation(EItemType ItemType, int32 SlotIndex)
{
	UHDAbilitySystemComponent* HDASC = Cast<UHDAbilitySystemComponent>(GetAbilitySystemComponent());
	UHDAttributeSet* HDAS = Cast<UHDAttributeSet>(AttributeSetBase);
	switch (ItemType)
	{
	case EItemType::EIT_Helmet:
		if (!IsValid(Helmet)) break;
		HelmetMesh->SetSkeletalMesh(nullptr);
		TotalArmor -= Helmet->ArmorIncrease;
		Helmet = nullptr;
		HDAS->SetArmor(TotalArmor);
		break;
	case EItemType::EIT_Chest:
		if (!IsValid(Chestplate)) break;
		ChestplateMesh->SetSkeletalMesh(nullptr);
		TotalArmor -= Chestplate->ArmorIncrease;
		Chestplate = nullptr;
		HDAS->SetArmor(TotalArmor);
		break;
	case EItemType::EIT_Arms:
		if (!IsValid(Gauntlets)) break;
		GauntletsMesh->SetSkeletalMesh(nullptr);
		TotalArmor -= Gauntlets->ArmorIncrease;
		Gauntlets = nullptr;
		HDAS->SetArmor(TotalArmor);
		break;
	case EItemType::EIT_Legs:
		if (!IsValid(Legs)) break;
		LegsMesh->SetSkeletalMesh(nullptr);
		TotalArmor -= Legs->ArmorIncrease;
		Legs = nullptr;
		HDAS->SetArmor(TotalArmor);
		break;
	case EItemType::EIT_Weapon:
		switch (SlotIndex)
		{
		case 1:
			if (EquippedWeapon1)
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(EquippedWeapon1->DefaultAbility, 1);
				for (auto ActiveAbility : HDASC->GetActivatableAbilities())
				{
					if (ActiveAbility.Ability->AbilityTags == AbilitySpec.Ability->AbilityTags)
					{
						HDASC->ClearAbility(ActiveAbility.Handle);
					}
				}
				EquippedWeapon1->Destroy();
				EquippedWeapon1 = nullptr;
				if (!IsValid(EquippedOffhandItem) || !EquippedOffhandItem.IsA(ABaseWeapon::StaticClass()))
				{
					SetCombatStyle(ECombatStyle::ECS_Unarmed);
				}
			}
			break;
		case 2:
			if (EquippedOffhandItem)
			{
				EquippedOffhandItem->Destroy();
				EquippedOffhandItem = nullptr;
				if (!IsValid(EquippedWeapon1) || !EquippedWeapon1.IsA(ABaseWeapon::StaticClass()))
				{
					SetCombatStyle(ECombatStyle::ECS_Unarmed);
				}
			}
			break;
		default: ;
		}
		break;
	case EItemType::EIT_Consumable:
		break;
	case EItemType::EIT_Amulet:
		switch (SlotIndex)
		{
		case 1:
			if (EquippedAmulet1)
			{
				EquippedAmulet1->Destroy();
				EquippedAmulet1 = nullptr;
			}
			break;
		case 2:
			if (EquippedAmulet2)
			{
				EquippedAmulet2->Destroy();
				EquippedAmulet2 = nullptr;
			}
			break;
		case 3:
			if (EquippedAmulet3)
			{
				EquippedAmulet3->Destroy();
				EquippedAmulet3 = nullptr;
			}
			break;
	default: ;
		}
		
		break;
	default: ;
	}
}

void APlayerCharacter::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	if (EquippedWeapon1)
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult HitResult;
		const FVector SocketLocation = EquippedWeapon1->GetItemMesh()->GetSocketLocation(FName("TipSocket"));
		UKismetSystemLibrary::SphereTraceSingle(this, SocketLocation, BeamTargetLocation, 10.f, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
		if (HitResult.bBlockingHit)
		{
			//if (!HasAuthority()) TargetActor = HitResult.GetActor();
			ServerSetImpactPoint(HitResult.ImpactPoint);
		}
	}
}

void APlayerCharacter::UpdateLightVignette()
{
	if (LightingComponent && FollowCamera)
	{
		float LightLevel = LightingComponent->GetLightingAmount();
		if (LightLevel < 0.04f)
		{
			FollowCamera->SetPostProcessBlendWeight(UKismetMathLibrary::Lerp(1,0,(LightLevel * 80.f)));
		}
		else
		{
			FollowCamera->SetPostProcessBlendWeight(0.f);
		}
	}
}

void APlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	return PS->AddToXP(InXP);
}

void APlayerCharacter::MultiCastLevelUpParticles_Implementation() const
{
	if(IsValid(LevelUpNiagaraComponent))
	{
		LevelUpNiagaraComponent->Activate(true);
	}
}

void APlayerCharacter::LevelUp_Implementation()
{
	MultiCastLevelUpParticles();
}

int32 APlayerCharacter::GetXP_Implementation() const
{
	const ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	return PS->GetXP();
}

int32 APlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	return PS->LevelUpInfo->FindLevelForXP(InXP);
}

int32 APlayerCharacter::GetAttributesPointsReward_Implementation(int32 Level) const
{
	const ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	return PS->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 APlayerCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	return PS->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void APlayerCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	PS->AddToLevel(InPlayerLevel);

	if (UHDAbilitySystemComponent* HDASC = Cast<UHDAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		HDASC->UpdateAbilityStatuses(PS->GetPlayerLevel());
	}
}

void APlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	PS->AddToAttributePoints(InAttributePoints);
}

void APlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	PS->AddToSpellPoints(InSpellPoints);
}

int32 APlayerCharacter::GetAttributePoints_Implementation() const
{
	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	return PS->GetAttributePoints();
}

int32 APlayerCharacter::GetSpellPoints_Implementation() const
{
	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	check(PS);
	return PS->GetSpellPoints();
}

void APlayerCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetController()))
	{
		PlayerController->ShowMagicCircle(DecalMaterial);
	}
}

void APlayerCharacter::HideMagicCircle_Implementation()
 {
	if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(GetController()))
	{
		PlayerController->HideMagicCircle();
	}
 }

UInventoryComponent* APlayerCharacter::GetInsuranceInventoryComponent_Implementation()
{
	return InsuranceInventoryComponent;
}

FHitResult APlayerCharacter::GetTraceResult(float TraceDistance, bool bUseVisibilityChannel)
{
	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult, TraceDistance, bUseVisibilityChannel);
	return HitResult;
}

int32 APlayerCharacter::GetPlayerLevel_Implementation()
{
	const ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (!IsValid(PS)) return 1; //check(PS);
	return PS->GetPlayerLevel();
}

void APlayerCharacter::SetStepping_Implementation(bool bStepping)
{
	bIsStepping = bStepping;
}

FVector APlayerCharacter::GetImpactPoint()
{
	return ImpactPoint;
}

void APlayerCharacter::OnRep_Stunned()
{
	if (UHDAbilitySystemComponent* HDASC = Cast<UHDAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FHDGameplayTags& GameplayTags = FHDGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			HDASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			HDASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void APlayerCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void APlayerCharacter::ServerSetImpactPoint_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastSetImpactPoint(TraceHitTarget);
}

void APlayerCharacter::MulticastSetImpactPoint_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	ImpactPoint = TraceHitTarget;
}


