
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Abilities/Data/CharacterClassInfo.h"
#include "BasePlayerController.generated.h"

class AEnemySpawnPoint;
class AHealthCharacter;
class AEquippableActor;

USTRUCT(BlueprintType)
struct FUserSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float XSens = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float YSens = 1.f;

	void operator=(const FUserSettings & Other)
	{
		this->XSens = Other.XSens;
		this->YSens = Other.YSens;
	}
};

class ABaseEnemy;
class AMagicCircle;
class APlayerCharacter;
class UDamageTextComponent;
class UHDInputConfig;
class ABaseHUD;
class UMainHUD;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UHDAbilitySystemComponent;

UCLASS()
class HORRORDUNGEON_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ABasePlayerController();
	/* Player Input */
	virtual void SetupInputComponent() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Crouch();
	void Interact();
	void DisplaySpellAttributesMenu();
	void DisplayInventory();
	void DisplayOptionsMenu();
	UFUNCTION(BlueprintCallable)
	void DisplayTeleportMenu(TSubclassOf<UMainHUD> TeleportMenu);

	void HideWidgets(UUserWidget* WidgetToKeepOpen);
	
	void AimButtonPressed();
	void AimButtonReleased();
	UFUNCTION(BlueprintCallable)
	void SprintButtonPressed();
	UFUNCTION(BlueprintCallable)
	void SprintButtonReleased();

	void UseManaPotion();

	void InitializeHUDWidgets();

	void PlayerDied();
	
	/* HUD */
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	
	UPROPERTY(EditAnywhere, Category= "HUD")
	TSubclassOf<UMainHUD> SpellAttributesMenu;
	UPROPERTY(EditAnywhere, Category= "HUD")
	TSubclassOf<UMainHUD> Inventory;
	UPROPERTY(EditAnywhere, Category= "HUD")
	TSubclassOf<UMainHUD> OptionsMenu;
	UPROPERTY(EditAnywhere, Category= "HUD")
	TSubclassOf<UMainHUD> Crosshair;
	
	UPROPERTY()
	UUserWidget* SpellAttributesWidget;
	UPROPERTY()
	UUserWidget* OptionsWidget;
	UPROPERTY()
	UUserWidget* InventoryWidget;
	UPROPERTY()
	UUserWidget* CrosshairWidget;
	UPROPERTY()
	UUserWidget* TeleportWidget;

	

	bool bSpellAttributesMenuInitialized = false;
	bool bInventoryInitialized = false;
	bool bOptionsMenuInitialized = false;
	bool bCrosshairInitialized = false;
	bool bTeleportMenuInitialized = false;

	bool bHUDInitialized = false;

	bool bSpellAttributesMenuOpen = false;
	UPROPERTY(BlueprintReadWrite)
	bool bOptionsMenuOpen = false;
	bool bInventoryOpen = false;
	UPROPERTY(BlueprintReadWrite)
	bool bTeleportMenuOpen = false;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FUserSettings UserSettings;

	UFUNCTION(BlueprintCallable)
	void SaveSettings();
	UFUNCTION(BlueprintCallable)
	void LoadSettings();
protected:
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	void SpawnActor(ECharacterClass InCharacterClass);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> MageCharacter;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> KnightCharacter;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> RangerCharacter;

	/* Teleporting and Extracting */
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerStartTeleportSequence(FName InMapName);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerSpawnExtractionEnemies();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EnemySpawnPointsClass;
	
	UPROPERTY()
	TArray<AActor*> ESpawnPoints;

	UFUNCTION()
	void SpawnEnemies();
	
	/*  */

	/* Inventory/Interaction */

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddItemToInventory();
	void InteractWithObject(UObject* Object);
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AEquippableActor> EquippableActor;

	/*  */
	
	/* Revive */
	UFUNCTION(Server, Reliable)
	void ServerCallRevive(APlayerCharacter* InPlayerToRevive);
	UFUNCTION(NetMulticast, Reliable)
	void MultiCastCallRevive();
	UFUNCTION(NetMulticast, Reliable)
	void ReviveCompleted(FName NotifyName);
	UFUNCTION(Server, Reliable)
	void ServerReviveInterrupted();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastReviveInterrupted();
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* StartRevive;

	UPROPERTY()
	APlayerCharacter* PlayerToRevive;

	/* End Revive */

private:
	UPROPERTY()
	TObjectPtr<ABaseHUD> BaseHUD;
	
	/* Player Input */
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UHDInputConfig> InputConfig;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> PlayerMappingContext;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MovementAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> SpellAttributeMenuAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> OptionsMenuAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> InventoryAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AimingAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> ManaPotionAction;

	UHDAbilitySystemComponent* GetASC();
	
	UPROPERTY()
	TObjectPtr<UHDAbilitySystemComponent> HDAbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	void UpdateMagicCircleLocation();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;
	
	ECharacterClass PlayerCharacterClass = {};

	FGameplayTagContainer BlockInputTags;



	UPROPERTY()
	TObjectPtr<AHealthCharacter> HealthCharacter;
public:

	UFUNCTION(Server, Reliable)
	void ServerSpawnActor(ECharacterClass InCharacterClass);
};
