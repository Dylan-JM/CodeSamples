﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignInOverlay.generated.h"

class USignInPage;
class USignUpPage;
class UConfirmSignUpPage;
class USuccessConfirmedPage;
class UWidgetSwitcher;
class UPortalManager;
class UJoinGame;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API USignInOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

protected:

	virtual void NativeConstruct() override;
	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignInPage> SignInPage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignUpPage> SignUpPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UConfirmSignUpPage> ConfirmSignUpPage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USuccessConfirmedPage> SuccessConfirmedPage;
	
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	void AutoSignIn();
	
	UFUNCTION()
	void ShowSignInPage();

	UFUNCTION()
	void ShowSignUpPage();
	
	UFUNCTION()
	void ShowConfirmSignUpPage();
	
	UFUNCTION()
	void ShowSuccessConfirmedPage();

	UFUNCTION()
	void SignInButtonClicked();
	
	UFUNCTION()
	void SignUpButtonClicked();

	UFUNCTION()
	void ConfirmButtonClicked();

	UFUNCTION()
	void OnSignupSucceeded();

	UFUNCTION()
	void OnConfirmSucceeded();
};

