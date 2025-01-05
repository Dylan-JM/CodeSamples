// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HUDManagement.generated.h"


UINTERFACE()
class UHUDManagement : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEDICATEDSERVERS_API IHUDManagement
{
	GENERATED_BODY()

public:

	virtual void OnSignIn() = 0;
	virtual void OnSignOut() = 0;
};
