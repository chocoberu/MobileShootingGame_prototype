// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "UI/RightButtonHUDWidget.h"
#include "UI/SWeaponStatusWidget.h"

void ASPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	RightButtonHUD = CreateWidget<URightButtonHUDWidget>(this, RightPadButtonHUDClass);
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("RightButtonHUD is nullptr"));
		return;
	}
	WeaponStatusWidget = CreateWidget<USWeaponStatusWidget>(this, WeaponStatusWidgetClass);
	if (nullptr == WeaponStatusWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponStatusWidget is nullptr"));
		return;
	}
}

void ASPlayerController::BindMainWeaponStatusWidget(ASWeapon* MainWeapon)
{
	UE_LOG(LogTemp, Log, TEXT("BindMainWeaponStatusWidget() Call"));
	if (nullptr == WeaponStatusWidget)
	{
		return;
	}

	WeaponStatusWidget->BindMainWeapon(MainWeapon);
	UE_LOG(LogTemp, Log, TEXT("BindMainWeaponStatusWidget() Complete"));
}

void ASPlayerController::BindSubWeaponStatusWidget(ASSubWeapon* SubWeapon)
{
	UE_LOG(LogTemp, Log, TEXT("BindSubWeaponStatusWidget() Call"));
	if (nullptr == WeaponStatusWidget)
	{
		return;
	}

	WeaponStatusWidget->BindSubWeapon(SubWeapon);
	UE_LOG(LogTemp, Log, TEXT("BindSubWeaponStatusWidget() Complete"));
}

void ASPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	RightButtonHUD->AddToViewport();
	WeaponStatusWidget->AddToViewport();
}