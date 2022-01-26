// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "UI/RightButtonHUDWidget.h"
#include "UI/SWeaponStatusWidget.h"
#include "UI/SPraticeMenuWidget.h"
#include "Kismet/GameplayStatics.h"

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
	
	if (nullptr == MenuWidgetClass)
	{
		return;
	}

	MenuWidget = CreateWidget<USPraticeMenuWidget>(this, MenuWidgetClass);
	if (nullptr != MenuWidget)
	{
		MenuWidget->OnResumeDelegate.AddUObject(this, &ASPlayerController::OnGameResume);
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

void ASPlayerController::OnGamePause()
{
	if (nullptr == MenuWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Menu Widget is nullptr"));
		return;
	}
	SetPause(true);

	MenuWidget->AddToViewport();
	bShowMouseCursor = true;
}

void ASPlayerController::OnGameResume()
{
	if (nullptr == MenuWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Menu Widget is nullptr"));
		return;
	}
	SetPause(false);

	MenuWidget->RemoveFromViewport();
	bShowMouseCursor = false;
}

void ASPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentPlatform = UGameplayStatics::GetPlatformName();

	//if (0 == CurrentPlatform.Compare(TEXT("IOS")) || 0 == CurrentPlatform.Compare(TEXT("Android")))
	{
		RightButtonHUD->AddToViewport();
		if (nullptr == MenuWidget)
		{
			RightButtonHUD->SetHiddenMenuButton(true);
		}
	}
	WeaponStatusWidget->AddToViewport();
}