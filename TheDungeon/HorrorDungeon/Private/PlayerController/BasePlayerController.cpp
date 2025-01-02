

#include "PlayerController/BasePlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HDGameplayTags.h"
#include "HorrorDungeonGameMode.h"
#include "InputActionValue.h"
#include "PlayMontageCallbackProxy.h"
#include "Actor/EnemySpawnPoint.h"
#include "Actor/MagicCircle.h"
#include "Actor/Teleporter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/HDAbilitySystemComponent.h"
#include "Game/HDGameInstance.h"
#include "Game/UserSettingsSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "HUD/BaseHUD.h"
#include "HUD/Widget/DamageTextComponent.h"
#include "HUD/Widget/MainHUDWidget.h"
#include "Input/HDInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Lighting/HDTorch.h"
#include "PlayerCharacter/HealthCharacter.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "PlayerState/BasePlayerState.h"
#include "UserSettings/EnhancedInputUserSettings.h"

ABasePlayerController::ABasePlayerController()
{
	bReplicates = true;

	BlockInputTags.AddTag(FHDGameplayTags::Get().Player_Block_InputHeld);
	//BlockInputTags.AddTag(FHDGameplayTags::Get().Player_Block_InputReleased);
	BlockInputTags.AddTag(FHDGameplayTags::Get().Player_Block_InputPressed);
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(PlayerMappingContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
		if (!Subsystem->GetUserSettings()->IsMappingContextRegistered(PlayerMappingContext))
		{
			Subsystem->GetUserSettings()->RegisterInputMappingContext(PlayerMappingContext);
		}
	}
	BaseHUD = Cast<ABaseHUD>(GetHUD());
	
	if (IsLocalPlayerController())
	{
		UHDGameInstance* HDGameInstance = Cast<UHDGameInstance>(GetGameInstance());
		ServerSpawnActor(HDGameInstance->CharacterClass);
	}

	HealthCharacter = Cast<AHealthCharacter>(UGameplayStatics::GetActorOfClass(this, AHealthCharacter::StaticClass()));
}

void ABasePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	UpdateMagicCircleLocation();
}

void ABasePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
	LoadSettings();
}

void ABasePlayerController::SpawnActor(ECharacterClass InCharacterClass)
{
	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass()));

	switch (InCharacterClass) {
		case ECharacterClass::Elementalist:
			PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(MageCharacter, PlayerStart->GetActorTransform());
			break;
		case ECharacterClass::Warrior:
			PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(KnightCharacter, PlayerStart->GetActorTransform());
			break;

		case ECharacterClass::Ranger:
			PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(RangerCharacter, PlayerStart->GetActorTransform());
			break;

		case ECharacterClass::Boss:
			PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(MageCharacter, PlayerStart->GetActorTransform());
			break;
		default:
			PlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(MageCharacter, PlayerStart->GetActorTransform());
	}
	
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UHDInputComponent* HDInputComponent = CastChecked<UHDInputComponent>(InputComponent);
	HDInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
	HDInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Look);
	HDInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Crouch);
	HDInputComponent->BindAction(SpellAttributeMenuAction, ETriggerEvent::Triggered, this, &ABasePlayerController::DisplaySpellAttributesMenu);
	HDInputComponent->BindAction(OptionsMenuAction, ETriggerEvent::Triggered, this, &ABasePlayerController::DisplayOptionsMenu);
	HDInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &ABasePlayerController::DisplayInventory);
	HDInputComponent->BindAction(AimingAction, ETriggerEvent::Triggered, this, &ABasePlayerController::AimButtonPressed);
	HDInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &ABasePlayerController::AimButtonReleased);
	HDInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Interact);
	HDInputComponent->BindAction(InteractAction, ETriggerEvent::Canceled, this, &ABasePlayerController::ServerReviveInterrupted);
	HDInputComponent->BindAction(ManaPotionAction, ETriggerEvent::Triggered, this, &ABasePlayerController::UseManaPotion);

	HDInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
	
}

void ABasePlayerController::Move(const FInputActionValue& Value)
{
	if (PlayerCharacter && PlayerCharacter->bDead) return;
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FHDGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (PlayerCharacter == nullptr) PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	
	if (PlayerCharacter)
	{
		PlayerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		PlayerCharacter->AddMovementInput(RightDirection, MovementVector.X);
		PlayerCharacter->MovementDirectionVector = MovementVector;
	}
}

void ABasePlayerController::Look(const FInputActionValue& Value)
{
	if (bSpellAttributesMenuOpen || bInventoryOpen) return;
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (PlayerCharacter)
	{
		PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y * UserSettings.YSens);
		PlayerCharacter->AddControllerYawInput(LookAxisVector.X * UserSettings.YSens);
	}
}

void ABasePlayerController::Crouch()
{
	if (PlayerCharacter->bIsCrouched)
	{
		PlayerCharacter->UnCrouch(true);
	}
	else
	{
		PlayerCharacter->Crouch(true);
	}
}

void ABasePlayerController::Interact()
{
	if (!IsValid(PlayerCharacter) || !IsValid(PlayerCharacter->GetThisActor())) return;
	EquippableActor = Cast<AEquippableActor>(PlayerCharacter->GetThisActor());
	PlayerToRevive = Cast<APlayerCharacter>(PlayerCharacter->GetThisActor());
	if (EquippableActor)
	{
		AddItemToInventory();
	}
	else if (PlayerToRevive)
	{
		if (PlayerToRevive->bDeadOnGround && HealthCharacter->bCanRevive)
		{
			ServerCallRevive(PlayerToRevive);
		}
	}
	else
	{
		InteractWithObject(PlayerCharacter->GetThisActor());
	}
}

void ABasePlayerController::PlayerDied()
{
	ServerSpawnActor(PlayerCharacterClass);
}

void ABasePlayerController::InteractWithObject(UObject* Object)
{
	if (ATeleporter* Teleporter = Cast<ATeleporter>(Object))
	{
		if (Teleporter->bStartTeleporting)
		{
			return;
		}
		DisplayTeleportMenu(Teleporter->TeleportMenu);
	}
	if (AHDTorch* Torch = Cast<AHDTorch>(Object))
	{
		Torch->ToggleLight(true);
	}
}

void ABasePlayerController::ServerCallRevive_Implementation(APlayerCharacter* InPlayerToRevive)
{
	PlayerToRevive = InPlayerToRevive;
	MultiCastCallRevive();
}

void ABasePlayerController::MultiCastCallRevive_Implementation()
{
	UPlayMontageCallbackProxy* PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(PlayerCharacter->GetMesh(), StartRevive);
	PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &ABasePlayerController::ReviveCompleted);

	GetASC()->AddLooseGameplayTags(BlockInputTags);
}

void ABasePlayerController::ReviveCompleted_Implementation(FName NotifyName)
{
	if (AHorrorDungeonGameMode* HDGamemode = Cast<AHorrorDungeonGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (HDGamemode->HealthCharacter && HDGamemode->HealthCharacter->bCanRevive)
		{
			HDGamemode->RevivePlayer(PlayerToRevive);
			FGameplayTagContainer GameplayTagContainer;
		}
	}
	GetASC()->RemoveLooseGameplayTags(BlockInputTags);
}

void ABasePlayerController::ServerReviveInterrupted_Implementation()
{
	MulticastReviveInterrupted();
}

void ABasePlayerController::MulticastReviveInterrupted_Implementation()
{
	PlayerCharacter->StopAnimMontage(StartRevive);
	
	GetASC()->RemoveLooseGameplayTags(BlockInputTags);
}

void ABasePlayerController::InitializeHUDWidgets()
{
	SpellAttributesWidget = CreateWidget(this, SpellAttributesMenu);
	SpellAttributesWidget->AddToViewport();
	SpellAttributesWidget->SetVisibility(ESlateVisibility::Collapsed);
	bSpellAttributesMenuInitialized = true;
	
	InventoryWidget = CreateWidget(this, Inventory);
	InventoryWidget->AddToViewport();
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryInitialized = true;

	OptionsWidget = CreateWidget(this, OptionsMenu);
	OptionsWidget->AddToViewport();
	OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
	bOptionsMenuInitialized = true;

	CrosshairWidget = CreateWidget(this, Crosshair);
	CrosshairWidget->AddToViewport();
	CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
	bCrosshairInitialized = true;
	
	bHUDInitialized = true;
}

void ABasePlayerController::DisplaySpellAttributesMenu()
{
	if (!bHUDInitialized)
	{
		InitializeHUDWidgets();
	}
	if (bSpellAttributesMenuInitialized && !bSpellAttributesMenuOpen)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		SetShowMouseCursor(true);
		SpellAttributesWidget->SetVisibility(ESlateVisibility::Visible);
		bSpellAttributesMenuOpen = true;

		HideWidgets(SpellAttributesWidget);
	}
	
	else if (bSpellAttributesMenuInitialized && bSpellAttributesMenuOpen)
	{
		SpellAttributesWidget->SetVisibility(ESlateVisibility::Collapsed);
		bSpellAttributesMenuOpen = false;
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		SetShowMouseCursor(false);
	}
}

void ABasePlayerController::DisplayInventory()
{
	if (PlayerCharacter && PlayerCharacter->bDead) return;
	if (!bHUDInitialized)
	{
		InitializeHUDWidgets();
	}
	if (bInventoryInitialized && !bInventoryOpen)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		SetShowMouseCursor(true);
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		bInventoryOpen = true;

		HideWidgets(InventoryWidget);
	}
	else if (bInventoryInitialized && bInventoryOpen)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		bInventoryOpen = false;
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		SetShowMouseCursor(false);
	}
}

void ABasePlayerController::DisplayOptionsMenu()
{
	if (!bHUDInitialized) InitializeHUDWidgets();

	if (bOptionsMenuInitialized && !bOptionsMenuOpen)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		SetShowMouseCursor(true);
		OptionsWidget->SetVisibility(ESlateVisibility::Visible);
		bOptionsMenuOpen = true;
		
		HideWidgets(OptionsWidget);
	}
	else if (bOptionsMenuInitialized && bOptionsMenuOpen)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		SetShowMouseCursor(false);
		OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
		bOptionsMenuOpen = false;
	}
}

void ABasePlayerController::DisplayTeleportMenu(TSubclassOf<UMainHUD> TeleportMenu)
{
	if (PlayerCharacter && PlayerCharacter->bDead) return;
	if (!bTeleportMenuInitialized)
	{
		TeleportWidget = CreateWidget(this, TeleportMenu);
		TeleportWidget->AddToViewport();
		TeleportWidget->SetVisibility(ESlateVisibility::Collapsed);
		bTeleportMenuInitialized = true;
	}
	if (bTeleportMenuInitialized && !bTeleportMenuOpen)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this);
		SetShowMouseCursor(true);
		TeleportWidget->SetVisibility(ESlateVisibility::Visible);
		bTeleportMenuOpen = true;

		HideWidgets(TeleportWidget);
	}
}

void ABasePlayerController::HideWidgets(UUserWidget* WidgetToKeepOpen)
{
	if (bSpellAttributesMenuOpen && SpellAttributesWidget && WidgetToKeepOpen != SpellAttributesWidget)
	{
		SpellAttributesWidget->SetVisibility(ESlateVisibility::Collapsed);
		bSpellAttributesMenuOpen = false;
	}
	if (bInventoryOpen && InventoryWidget && WidgetToKeepOpen != InventoryWidget)
	{
		InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		bInventoryOpen = false;
	}
	if (bOptionsMenuOpen && OptionsWidget && WidgetToKeepOpen != OptionsWidget)
	{
		OptionsWidget->SetVisibility(ESlateVisibility::Collapsed);
		bOptionsMenuOpen = false;
	}
	if (bTeleportMenuOpen && TeleportWidget && WidgetToKeepOpen != TeleportWidget)
	{
		TeleportWidget->SetVisibility(ESlateVisibility::Collapsed);
		bTeleportMenuOpen = false;
	}
}

void ABasePlayerController::AimButtonPressed()
{
	if (PlayerCharacter && PlayerCharacter->bDead) return;
	if (!bHUDInitialized)
	{
		InitializeHUDWidgets();
	}
	if (PlayerCharacter)
	{
		PlayerCharacter->SetAiming(true);
	}
}

void ABasePlayerController::AimButtonReleased()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->SetAiming(false);
	}
}

void ABasePlayerController::SprintButtonPressed()
{
	if (PlayerCharacter && PlayerCharacter->bDead) return;
	if (PlayerCharacter)
	{
		PlayerCharacter->SetSprinting(true);
		if (PlayerCharacter->bIsCrouched)
		{
			PlayerCharacter->UnCrouch(true);
		}
	}
}

void ABasePlayerController::SprintButtonReleased()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->SetSprinting(false);
	}
}

void ABasePlayerController::UseManaPotion()
{
	if (PlayerCharacter)
	{
		PlayerCharacter->UseManaPotion();
	}
}

void ABasePlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void ABasePlayerController::ServerStartTeleportSequence_Implementation(FName InMapName)
{
	ATeleporter* Teleporter = Cast<ATeleporter>(UGameplayStatics::GetActorOfClass(this, ATeleporter::StaticClass()));
	Teleporter->MultiCastStartTeleportAnimation();
	Teleporter->ServerStartTeleportSequence(InMapName);
}

void ABasePlayerController::ServerSpawnExtractionEnemies_Implementation()
{
	UGameplayStatics::GetAllActorsOfClass(this, EnemySpawnPointsClass, ESpawnPoints);
	FTimerHandle SpawnExtractionHandle;
	GetWorld()->GetTimerManager().SetTimer(SpawnExtractionHandle, this, &ABasePlayerController::SpawnEnemies, 3.f, true);
}


void ABasePlayerController::SpawnEnemies()
{
	for (AActor* SpawnPoint : ESpawnPoints)
	{
		if (AEnemySpawnPoint* ESpawnPoint = Cast<AEnemySpawnPoint>(SpawnPoint))
		{
			ESpawnPoint->SpawnExtractionEnemy();
		}
	}
}

void ABasePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (PlayerCharacter && PlayerCharacter->bDead) return;
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FHDGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ABasePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (PlayerCharacter && PlayerCharacter->bDead) return;
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FHDGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (bSpellAttributesMenuOpen || bInventoryOpen|| bOptionsMenuOpen || bTeleportMenuOpen) return;
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ABasePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (PlayerCharacter && PlayerCharacter->bDead) return;
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FHDGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if (bSpellAttributesMenuOpen || bInventoryOpen|| bOptionsMenuOpen || bTeleportMenuOpen) return;
	if (GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UHDAbilitySystemComponent* ABasePlayerController::GetASC()
{
	if (HDAbilitySystemComponent == nullptr)
	{
		HDAbilitySystemComponent = Cast<UHDAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return HDAbilitySystemComponent;
}

void ABasePlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle) && PlayerCharacter)
	{
		MagicCircle->SetActorLocation(PlayerCharacter->GetTraceResult(10000.f).ImpactPoint);
	}
}

void ABasePlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void ABasePlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}


void ABasePlayerController::SaveSettings()
{
	UUserSettingsSaveGame* SaveGame = Cast<UUserSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UUserSettingsSaveGame::StaticClass()));
	SaveGame->UserSettings = UserSettings;

	// Save
	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("UserSettings"), 0);
}

void ABasePlayerController::LoadSettings()
{
	UUserSettingsSaveGame* SaveGame = Cast<UUserSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UUserSettingsSaveGame::StaticClass()));
	SaveGame = Cast<UUserSettingsSaveGame>(UGameplayStatics::LoadGameFromSlot("UserSettings", 0));
	if(SaveGame != nullptr)
	{
		UserSettings = SaveGame->UserSettings;
	}
}

void ABasePlayerController::ServerSpawnActor_Implementation(ECharacterClass InCharacterClass)
{
	PlayerCharacterClass = InCharacterClass;
	SpawnActor(PlayerCharacterClass);
	if (PlayerCharacter)
	{
		Possess(PlayerCharacter);
	}
	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
}