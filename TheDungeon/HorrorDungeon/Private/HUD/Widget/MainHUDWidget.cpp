// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widget/MainHUDWidget.h"

void UMainHUD::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
