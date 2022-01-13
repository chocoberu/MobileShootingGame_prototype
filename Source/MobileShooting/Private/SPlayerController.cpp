// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "UI/RightButtonHUDWidget.h"
#include "UI/SWeaponStatusWidget.h"

void ASPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ASPlayerController::BindMainWeaponStatusWidget(ASWeapon* MainWeapon)
{
	if (nullptr == WeaponStatusWidget)
	{
		return;
	}

	WeaponStatusWidget->BindMainWeapon(MainWeapon);
}

void ASPlayerController::BindSubWeaponStatusWidget(ASSubWeapon* SubWeapon)
{
	if (nullptr == WeaponStatusWidget)
	{
		return;
	}

	WeaponStatusWidget->BindSubWeapon(SubWeapon);
}

void ASPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Log, TEXT("CreateWidget"));
	RightButtonHUD = CreateWidget<URightButtonHUDWidget>(this, RightPadButtonHUDClass);
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("RightButtonHUD is nullptr"));
		return;
	}

	RightButtonHUD->AddToViewport();
	
	WeaponStatusWidget = CreateWidget<USWeaponStatusWidget>(this, WeaponStatusWidgetClass);
	if (nullptr == WeaponStatusWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponStatusWidget is nullptr"));
		return;
	}

	WeaponStatusWidget->AddToViewport();
}