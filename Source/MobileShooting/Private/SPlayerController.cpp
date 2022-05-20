// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SGameInstance.h"
#include "TestSaveGame.h"
#include "UI/RightButtonHUDWidget.h"
#include "UI/SPraticeMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void ASPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::OnPossess() call"));

	// TEST CODE
	/*if (false == IsLocalPlayerController())
	{
		return;
	}*/

	/*if (nullptr != RightPadButtonHUDClass)
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
	}*/
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

	// TEST CODE
	FInputModeUIOnly Mode;
	SetInputMode(Mode);
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

	// TEST CODE
	FInputModeGameAndUI Mode;
	SetInputMode(Mode);
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

	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BeginPlay() call"));

	// TEST CODE
	if (false == IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("No Authority "));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Start Load Game"));

	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr != SPlayerState)
	{
		USGameInstance* SGameInstance = Cast<USGameInstance>(GetGameInstance());
		if (nullptr != SGameInstance)
		{
			UE_LOG(LogTemp, Log, TEXT("Current Player Name : %s"), *SGameInstance->GetCurrentPlayerName());
		}
		FString PlayerName = SGameInstance != nullptr ? SGameInstance->GetCurrentPlayerName() : TEXT("Player0");
		UTestSaveGame* SaveGame = Cast<UTestSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerName, 0));
		if (nullptr != SaveGame)
		{
			SPlayerState->SetPlayerName(PlayerName);
			SPlayerState->SetWeaponId(SaveGame->MainWeaponId);
			SPlayerState->SetSubWeaponId(SaveGame->SubWeaponId);
			SPlayerState->SetTeamNumber(SaveGame->TeamNum);

			UE_LOG(LogTemp, Log, TEXT("LoadGame Complete : %s"), *PlayerName);
		}
	}

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
