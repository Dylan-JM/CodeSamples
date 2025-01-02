
#include "PlayerCharacter/BaseCharacter.h"
#include "Engine.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "HDGameplayTags.h"
#include "Abilities/Debuffs/DebuffNiagaraComponent.h"
#include "Abilities/Passive/PassiveNiagaraComponent.h"
#include "Actor/HDEffectActor.h"
#include "Actor/Shield.h"
#include "Components/HDAbilitySystemComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/LightingComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Weapons/BaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HorrorDungeon/HorrorDungeon.h"
#include "Actor/EquippableActors/Armor.h"
#include "Net/UnrealNetwork.h"


ABaseCharacter::ABaseCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	
	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FHDGameplayTags::Get().Debuff_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = FHDGameplayTags::Get().Debuff_Stun;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Helmet");
	HelmetMesh->SetupAttachment(GetMesh());
	HelmetMesh->SetLeaderPoseComponent(GetMesh(), true);
	
	ChestplateMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Chestplate");
	ChestplateMesh->SetupAttachment(GetMesh());
	ChestplateMesh->SetLeaderPoseComponent(GetMesh(), true);

	GauntletsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Gauntlets");
	GauntletsMesh->SetupAttachment(GetMesh());
	GauntletsMesh->SetLeaderPoseComponent(GetMesh(), true);
	
	LegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Legs");
	LegsMesh->SetupAttachment(GetMesh());
	LegsMesh->SetLeaderPoseComponent(GetMesh(), true);
	
	bAlwaysRelevant = true;
	
	LightingComponent = CreateDefaultSubobject<ULightingComponent>(TEXT("LightingComponent"));
	
	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComponent->SetupAttachment(GetRootComponent());

	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloOfProtectionComponent");
	HaloOfProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);
	
	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonComponent");
	LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	
	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphonComponent");
	ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentManaPotions = MaxNumOfManaPotions;
}


void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, bIsBlocking);
	DOREPLIFETIME(ABaseCharacter, bIsStunned);
	DOREPLIFETIME(ABaseCharacter, bIsBurned);
	DOREPLIFETIME(ABaseCharacter, bIsBeingShocked);
	DOREPLIFETIME(ABaseCharacter, bIsAiming);
	DOREPLIFETIME(ABaseCharacter, bDeadOnGround);
	DOREPLIFETIME(ABaseCharacter, bDead);
	DOREPLIFETIME(ABaseCharacter, Helmet);
	DOREPLIFETIME(ABaseCharacter, Chestplate);
	DOREPLIFETIME(ABaseCharacter, Gauntlets);
	DOREPLIFETIME(ABaseCharacter, Legs);
	DOREPLIFETIME(ABaseCharacter, CombatStyle);
	DOREPLIFETIME(ABaseCharacter, EquippedWeapon1);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

void ABaseCharacter::SetCombatStyle(ECombatStyle InCombatStyle)
{
	CombatStyle = InCombatStyle;
}

void ABaseCharacter::SpawnWeapons()
{
	if (Weapon1 == nullptr) return;
	const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName(Weapon1.GetDefaultObject()->DefaultSocketName);
	if (RightHandSocket)
	{
		EquippedWeapon1 = Cast<ABaseWeapon>(GetWorld()->SpawnActor(Weapon1));
		if (EquippedWeapon1)
		{
			EquippedWeapon1->GetItemMesh()->SetSimulatePhysics(false);
			RightHandSocket->AttachActor(EquippedWeapon1, GetMesh());
			EquippedWeapon1->SetOwner(this);
			EquippedWeapon1->bOnGround = false;
			SetCombatStyle(EquippedWeapon1->CombatStyle);
		}
	}
	if (OffhandItem == nullptr) return;
	const USkeletalMeshSocket* LeftHandSocket = GetMesh()->GetSocketByName(OffhandItem.GetDefaultObject()->DefaultSocketName);
	if (LeftHandSocket && OffhandItem)
	{
		EquippedOffhandItem = Cast<AEquippableActor>(GetWorld()->SpawnActor(OffhandItem));
		if (EquippedOffhandItem)
		{
			EquippedOffhandItem->GetItemMesh()->SetSimulatePhysics(false);
			LeftHandSocket->AttachActor(EquippedOffhandItem, GetMesh());
			EquippedOffhandItem->SetOwner(this);
			EquippedOffhandItem->bOnGround = false;
		}
	}
}

void ABaseCharacter::AddCharacterAbilities()
{
	UHDAbilitySystemComponent* HDASC = CastChecked<UHDAbilitySystemComponent>(AbilitySystemComponent);
	// Grant abilities, but only on the server
	if (!HasAuthority()) return;
	
	HDASC->AddCharacterAbilities(StartupAbilities);
	HDASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void ABaseCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	// this line makes it so the enemy stand still when hit react happens
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	// my line in which the enemy doesn't stop but slows down, can change the division to variable if desired in future
	//GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? (BaseWalkSpeed/2) : BaseWalkSpeed;
}

void ABaseCharacter::HandleGameplayEffectRemoved(const FActiveGameplayEffectHandle InGEHandle)
{
	FOnActiveGameplayEffectRemoved_Info* DelPtr = AbilitySystemComponent->OnGameplayEffectRemoved_InfoDelegate(InGEHandle);
	if (DelPtr)
	{
		DelPtr->AddUObject(this, &ABaseCharacter::OnGameplayEffectRemoved);
	}
}

void ABaseCharacter::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& InGERemovalInfo)
{
	GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::Green, TEXT("OnGERemoved"));
	UHDAbilitySystemComponent* HDASC = CastChecked<UHDAbilitySystemComponent>(AbilitySystemComponent);
	FGameplayTagContainer EffectTags;
	InGERemovalInfo.ActiveEffect->Spec.GetAllGrantedTags(EffectTags);
	TArray<FGameplayTag> GameplayTags;
	EffectTags.GetGameplayTagArray(GameplayTags);
	HDASC->MulticastActivatePassiveEffect(GameplayTags[0], false);
}

void ABaseCharacter::UseManaPotion()
{
	if (CurrentManaPotions > 0)
	{
		CurrentManaPotions--;
		FActorSpawnParameters SpawnParams;
		AHDEffectActor* ManaPotion = GetWorld()->SpawnActorDeferred<AHDEffectActor>(ManaPotionClass, GetActorTransform(), this);
		if (ManaPotion)
		{
			ManaPotion->HighlightCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			ManaPotion->FinishSpawning(GetActorTransform());
		}
	}
}

void ABaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass)
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ABaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
	if (StaminaRegeneration)
	{
		ApplyEffectToSelf(StaminaRegeneration, 1.f);
	}
}

void ABaseCharacter::Die(const FVector& DeathImpulse, bool bRagdoll)
{
	MulticastHandleDeath(DeathImpulse, bRagdoll);
}

FOnDeath& ABaseCharacter::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

void ABaseCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse, bool bRagdoll)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	if (bRagdoll)
	{
		if (EquippedWeapon1)
		{
			EquippedWeapon1->OnOwnerDead(DeathImpulse);
		}
		if (EquippedOffhandItem)
		{
			EquippedOffhandItem->OnOwnerDead(DeathImpulse);
		}
	
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

		GetCapsuleComponent()->DestroyComponent();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bDeadOnGround = true;
		OnDeathDelegate.Broadcast(this);
	}
	else
	{
		if (DeathMontage)
		{
			PlayAnimMontage(DeathMontage);
		}
	}
	bDead = true;
	BurnDebuffComponent->Deactivate();
	StunDebuffComponent->Deactivate();
}


void ABaseCharacter::DeathMontageEnded()
{
	bDeadOnGround = true;
	OnDeathDelegate.Broadcast(this);
}

void ABaseCharacter::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

bool ABaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

AActor* ABaseCharacter::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ABaseCharacter::GetAbilityMontages_Implementation()
{
	return AttackMontages;
}

int32 ABaseCharacter::GetMinionCount_Implementation()
{
	return MinionCount;
}

void ABaseCharacter::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass ABaseCharacter::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered& ABaseCharacter::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

ABaseWeapon* ABaseCharacter::GetEquippedWeapon1_Implementation()
{
	return EquippedWeapon1;
}

ABaseWeapon* ABaseCharacter::GetEquippedOffhandWeapon_Implementation()
{
	ABaseWeapon* OffhandWeapon = Cast<ABaseWeapon>(EquippedOffhandItem);
	return OffhandWeapon;
}

void ABaseCharacter::SetIsBeingShocked_Implementation(bool bInShock)
{
	bIsBeingShocked = bInShock;
}

bool ABaseCharacter::IsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

FOnDamageSignature& ABaseCharacter::GetOnDamageSignature()
{
	return OnDamageDelegate;
}

bool ABaseCharacter::GetIsAttacking_Implementation()
{
	return bIsAttacking;
}

void ABaseCharacter::SetIsAttacking_Implementation(bool InIsAttacking)
{
	bIsAttacking = InIsAttacking;
}

bool ABaseCharacter::ReserveAttackToken_Implementation(int32 Amount)
{
	if (AttackTokensCount >= Amount)
	{
		AttackTokensCount = AttackTokensCount - Amount;
		return true;
	}
	return false;
}

void ABaseCharacter::ReturnAttackToken_Implementation(int32 Amount)
{
	AttackTokensCount = AttackTokensCount + Amount;
}

int32 ABaseCharacter::GetTeamNumber(AActor* Actor)
{
	if (ABaseCharacter* TargetActor = Cast<ABaseCharacter>(Actor))
	{
		return TargetActor->TeamNumber;
	}
	else
	{
		return 0;
	}
}

bool ABaseCharacter::OnSameTeam_Implementation(AActor* Actor)
{
	return GetTeamNumber(Actor) == GetTeamNumber(this);
}

void ABaseCharacter::SetIsBlocking_Implementation(bool bInBlock)
{
	bIsBlocking = bInBlock;
}

bool ABaseCharacter::GetIsBlocking_Implementation()
{
	return bIsBlocking;
}

float ABaseCharacter::GetBlockingMultiplier_Implementation()
{
	if (AShield* Shield = Cast<AShield>(EquippedOffhandItem))
	{
		return Shield->DamageConsumptionMultiplier;
	}
	else if (EquippedWeapon1)
	{
		return EquippedWeapon1->DamageConsumptionMultiplier;
	}
	return 1.f;
}

void ABaseCharacter::EnableWeaponCollision_Implementation(const FGameplayTag& MontageTag)
{
	const FHDGameplayTags& GameplayTags = FHDGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand) && EquippedWeapon1)
	{
		EquippedWeapon1->HandleCollision(true);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand) && EquippedOffhandItem)
	{
		EquippedOffhandItem->HandleCollision(true);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && EquippedWeapon1 && EquippedOffhandItem)
	{
		EquippedWeapon1->HandleCollision(true);
		EquippedOffhandItem->HandleCollision(true);
	}
}

void ABaseCharacter::DisableWeaponCollision_Implementation()
{
	if (EquippedWeapon1)
	{
		EquippedWeapon1->HandleCollision(false);
	}
	if (EquippedOffhandItem)
	{
		EquippedOffhandItem->HandleCollision(false);
	}
}

UAnimMontage* ABaseCharacter::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}


void ABaseCharacter::ChangeMovementSpeed(bool InAiming)
{
	if (InAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = CombatWalkSpeed;
		bUseControllerRotationYaw = bUseRotationYawWhenAiming;
		GetCharacterMovement()->bOrientRotationToMovement = !InAiming;
	}
	else if (!InAiming)
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}



void ABaseCharacter::InitAbilityActorInfo()
{
	
}

void ABaseCharacter::HighlightActor()
{
	if (!bDeadOnGround) return;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void ABaseCharacter::UnHighlightActor()
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(this))
	{
		if (Enemy->bHasItem)
		{
			GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_GOLD);
			return;
		}
	}
	GetMesh()->SetRenderCustomDepth(false);
}

UInventoryComponent* ABaseCharacter::GetInventoryComponent_Implementation()
{
	return InventoryComponent;
}

FText ABaseCharacter::GetActorName_Implementation()
{
	return ActorName;
}

void ABaseCharacter::OnRep_Helmet()
{
	if (Helmet)
	{
		HelmetMesh->SetSkeletalMesh(Helmet->ArmorMesh);
	}
	else
	{
		HelmetMesh->SetSkeletalMesh(nullptr);
	}
}

void ABaseCharacter::OnRep_Chestplate()
{
	if (Chestplate)
	{
		ChestplateMesh->SetSkeletalMesh(Chestplate->ArmorMesh);
	}
	else
	{
		ChestplateMesh->SetSkeletalMesh(nullptr);
	}
}

void ABaseCharacter::OnRep_Gauntlets()
{
	if (Gauntlets)
	{
		GauntletsMesh->SetSkeletalMesh(Gauntlets->ArmorMesh);
	}
	else
	{
		GauntletsMesh->SetSkeletalMesh(nullptr);
	}
}

void ABaseCharacter::OnRep_Legs()
{
	if (Legs)
	{
		LegsMesh->SetSkeletalMesh(Legs->ArmorMesh);
	}
	else
	{
		LegsMesh->SetSkeletalMesh(nullptr);
	}
}

void ABaseCharacter::OnRep_Stunned()
{
	
}

void ABaseCharacter::OnRep_Burned()
{
}

void ABaseCharacter::AddEquippedItemsToInventory_Implementation()
{
}
