// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "UI/RightButtonHUDWidget.h"
#include "UI/SPraticeMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void ASPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (nullptr != RightPadButtonHUDClass)
	{
		RightButtonHUD = CreateWidget<URightButtonHUDWidget>(this, RightPadButtonHUDClass);
	}
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("RightButtonHUD is nullptr"));
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
	if (nullptr == RightButtonHUD)
	{
		return;
	}
	RightButtonHUD->BindMainWeapon(MainWeapon);
}

void ASPlayerController::BindSubWeaponStatusWidget(ASSubWeapon* SubWeapon)
{
	if (nullptr == RightButtonHUD)
	{
		return;
	}
	RightButtonHUD->BindSubWeapon(SubWeapon);
}

void ASPlayerController::OnGamePause()
{
	if (nullptr == MenuWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("Menu Widget is nullptr"));
		return;
	}
	SetPause(true);
	RightButtonHUD->SetVisibleWeaponStatusAnimation(true);

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
	RightButtonHUD->SetVisibleWeaponStatusAnimation(false);

	MenuWidget->RemoveFromViewport();
	bShowMouseCursor = false;
}

void ASPlayerController::OnPlayerDead()
{
	// Stop All Widget Animations
	if (nullptr == RightButtonHUD)
	{
		return;
	}
	RightButtonHUD->StopAllWidgetAnimations();

	// TODO : SCharacter 사망 시 작업 추가
}

void ASPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentPlatform = UGameplayStatics::GetPlatformName();
	// 임시 주석 처리
	//if (0 == CurrentPlatform.Compare(TEXT("IOS")) || 0 == CurrentPlatform.Compare(TEXT("Android")))
	{
		if (nullptr == RightButtonHUD)
		{
			return;
		}
		RightButtonHUD->AddToViewport();
		if (nullptr == MenuWidget)
		{
			RightButtonHUD->SetHiddenMenuButton(true);
		}
	}
}
