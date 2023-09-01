// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

class UElimAnnouncement;
class UAnnouncement;
class UCharacterOverlay;


USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsCenter;
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsLeft;
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsRight;
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsTop;
	UPROPERTY()
	TObjectPtr<UTexture2D> CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;
};

UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
	void AddCharacterOverlay();
	UPROPERTY(EditAnywhere, Category="Player Stats")
	TSubclassOf<UUserWidget> CharacterOverlayClass;
	UPROPERTY()
	TObjectPtr<UCharacterOverlay> CharacterOverlay;
	UPROPERTY(EditAnywhere, Category=Announcements)
	TSubclassOf<UUserWidget> AnnouncementClass;
	UPROPERTY()
	TObjectPtr<UAnnouncement> Announcement;
	void AddAnnouncement();
	void AddElimAnnouncement(FString Attacker, FString Victim);
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<APlayerController> OwningPlayer;
	
	FHUDPackage HUDPackage;

	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UElimAnnouncement> ElimAnnouncementClass;
	UPROPERTY(EditAnywhere)
	float ElimAnnouncementTime = 2.5f;
	UFUNCTION()
	void ElimAnnouncementTimerFinished(UElimAnnouncement* MsgToRemove);
	UPROPERTY()
	TArray<UElimAnnouncement*> ElimMessages;
public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
