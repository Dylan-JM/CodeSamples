// Copyright DM

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Abilities/GameplayAbility.h"
#include "NewInputAbilityTest.generated.h"

/**
 * 
 */
UCLASS()
class HORRORDUNGEON_API UNewInputAbilityTest : public UGameplayAbility
{
	GENERATED_BODY()


public:

	UNewInputAbilityTest();

	// Tells an ability to activate immediately when it's granted. (Useful for passive abilities and abilities forced on others)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Custom Gameplay Ability")
	bool ActivateAbilityOnGranted = false;

	// Which Input Action to bind the activation event to.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	UInputAction* ActivationInputAction = nullptr;

	// What Input Action trigger type to listen to for input pressed events.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	ETriggerEvent InputPressedTriggerType = ETriggerEvent::Started;

	// What Input Action trigger type to listen to for input released events.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input")
	ETriggerEvent InputReleasedTriggerType = ETriggerEvent::Completed;

	// Returns the "Avatar Character" associated with this Gameplay Ability.
	// Will return null if the Avatar Actor does not derive from Character.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACharacter* GetAvatarCharacter() const { return AvatarCharacter.Get(); }

protected:

	// Keep a pointer to "Avatar Character" so we don't have to cast to Character in instanced abilities owned by a Character derived class.
	TWeakObjectPtr<ACharacter> AvatarCharacter = nullptr;
	
	// Think of this as "BeginPlay".
	// Add logic here that should run when the Ability is first initialized.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	// Called to bind Input Pressed and Input Released events to the Avatar Actor's Enhanced Input Component if it is reachable. 
	void SetupEnhancedInputBindings(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);

	// Called when the "Activation Input Action" is triggered.
	void HandleInputPressedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle);

	// Called when the "Activation Input Action" is completed.
	void HandleInputReleasedEvent(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle SpecHandle);

	// Override "OnRemoveAbility" to clean up Enhanced Input Bindings.
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
