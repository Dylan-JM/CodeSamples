
#include "PlayerCharacter/PlayerAnimInstance.h"
#include "KismetAnimationLibrary.h"
#include "Actor/Shield.h"
#include "PlayerCharacter/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Character == nullptr)
	{
		Character = Cast<ACharacter>(TryGetPawnOwner());
	}
	if (Character == nullptr) return;

	FVector Velocity = Character->GetMovementComponent()->Velocity;
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, GetOwningActor()->GetActorRotation());
	bIsInAir = Character->GetCharacterMovement()->IsFalling();
	bIsAccelerating = Character->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true:false;
	
	if (BaseCharacter)
	{
		CombatStyleAnim = BaseCharacter->CombatStyle;
		bUsingShield = BaseCharacter->EquippedOffhandItem.IsA(AShield::StaticClass());
		bIsBlocking = BaseCharacter->bIsBlocking;
		bIsDead = BaseCharacter->bDead;
		bAttacking = BaseCharacter->bIsAttacking;
		bCanRotate = BaseCharacter->bIsAiming && !bIsAccelerating && !bIsInAir && !bIsDead;
	}
	
	// Offset Yaw for strafing
	
	FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(Character->GetBaseAimRotation(), Character->GetActorRotation());
	DeltaYaw= DeltaRotator.Yaw;
	DeltaPitch = DeltaRotator.Pitch;

	/*DeltaRoll = DeltaRotator.Roll;*/
}
