// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SCharacter.h"
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
	if (false == bLoadPlayerState)
	{
		Client_LoadPlayerStateInfo();
	}

	ASCharacter* PlayerCharacter = Cast<ASCharacter>(aPawn);
	if (nullptr == PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::OnPossess(), PlayerCharacter is nullptr"));
		return;
	}

	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr != SPlayerState)
	{
		PlayerCharacter->SetGenericTeamId(SPlayerState->GetTeamNumber());
	}
	
	PlayerCharacter->LoadWeapon();
}

void ASPlayerController::BindMainWeaponStatusWidget(ASWeapon* MainWeapon)
{
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("ASPlayerController::BindMainWeaponStatusWidget(), RightButtonHUD is nullptr"));
		return;
	}
	RightButtonHUD->BindMainWeapon(MainWeapon);
}

void ASPlayerController::BindSubWeaponStatusWidget(ASSubWeapon* SubWeapon)
{
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("ASPlayerController::BindSubWeaponStatusWidget(), RightButtonHUD is nullptr"));
		return;
	}
	RightButtonHUD->BindSubWeapon(SubWeapon);
}

void ASPlayerController::OnGamePause()
{
	if (nullptr == MenuWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::OnGamePause(), Menu Widget is nullptr"));
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
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::OnGameResume(), Menu Widget is nullptr"));
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

	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::PostInitializeComponents() called"));
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BeginPlay() call"));

	// TEST CODE
	if (false == IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BeginPlay(), No Local Controller"));
		return;
	}

	// 멀티 플레이인 경우 GameMode에서 SetInputMode를 바꾸도록 설정
	if (GetNetMode() != ENetMode::NM_Standalone)
	{
		FInputModeUIOnly UIInputMode;
		SetInputMode(UIInputMode);
	}
	
	FString CurrentPlatform = UGameplayStatics::GetPlatformName();
	// 임시 주석 처리
	//if (0 == CurrentPlatform.Compare(TEXT("IOS")) || 0 == CurrentPlatform.Compare(TEXT("Android")))
	{
		SetVirtualJoystickVisibility(true);
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

	// TEST CODE
	//Server_ReadyGame(true);
}

void ASPlayerController::InitWidget()
{
	if (nullptr != RightPadButtonHUDClass)
	{
		RightButtonHUD = CreateWidget<URightButtonHUDWidget>(this, RightPadButtonHUDClass);
	}
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("RightButtonHUD is nullptr"));
		return;
	}

	if (nullptr != MenuWidgetClass)
	{
		MenuWidget = CreateWidget<USPraticeMenuWidget>(this, MenuWidgetClass);
	}
	if (nullptr != MenuWidget)
	{
		MenuWidget->OnResumeDelegate.AddUObject(this, &ASPlayerController::OnGameResume);
	}
}

void ASPlayerController::Client_LoadPlayerStateInfo_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::Client_LoadPlayerStateInfo() called"));

	// TEST CODE
	// BeginPlayer()에서 InitWidget() 호출 시 일부 플랫폼에서 HUD와 Weapon Bind가 제대로 되지 않음 (HUD == nullptr이여서)
	// OnPossess()는 클라이언트에서 호출되지 않으므로 다른 곳에서 생성 필요
	InitWidget();
	
	USGameInstance* SGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr != SGameInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("Current Player Name : %s"), *SGameInstance->GetCurrentPlayerName());
	}
	
	FString PlayerName = SGameInstance != nullptr ? SGameInstance->GetCurrentPlayerName() : TEXT("Player0");
	UTestSaveGame* SaveGame = Cast<UTestSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerName, 0));
	if (nullptr == SaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("ASPlayerController::Client_LoadPlayerStateInfo(), Failed to LoadGame"));
		return;
	}
	else
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
			if (nullptr == SPlayerState)
			{
				UE_LOG(LogTemp, Error, TEXT("ASPlayerState is nullptr"))
			}
			else
			{
				SPlayerState->SetPlayerName(PlayerName);
				SPlayerState->SetWeaponId(SaveGame->MainWeaponId);
				SPlayerState->SetSubWeaponId(SaveGame->SubWeaponId);
				SPlayerState->SetPlayerIndex(SaveGame->PlayerIndex);
			}
			bLoadPlayerState = true;
			RequestRestartPlayerDelegate.Broadcast(this);
		}
		else
		{
			Server_LoadPlayerStateInfo(PlayerName, SaveGame->MainWeaponId, SaveGame->SubWeaponId, SaveGame->PlayerIndex);
		}
	}
}

void ASPlayerController::Server_LoadPlayerStateInfo_Implementation(const FString& NewPlayerName, int32 NewWeaponId, int32 NewSubWeaponId, int32 NewPlayerIndex)
{
	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::Server_LoadPlayerStateInfo() called, PlayerName : %s"), *NewPlayerName);

	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("SPlayerState is nullptr"));
		return;
	}

	SPlayerState->SetPlayerName(NewPlayerName);
	SPlayerState->SetWeaponId(NewWeaponId);
	SPlayerState->SetSubWeaponId(NewSubWeaponId);
	SPlayerState->SetPlayerIndex(NewPlayerIndex);

	bLoadPlayerState = true;
	RequestRestartPlayerDelegate.Broadcast(this);
}

bool ASPlayerController::Server_LoadPlayerStateInfo_Validate(const FString& NewPlayerName, int32 NewWeaponId, int32 NewSubWeaponId, int32 NewPlayerIndex)
{
	return true;
}

void ASPlayerController::Server_ReadyGame_Implementation(bool bReadyState)
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("SPlayerState is nullptr"));
		return;
	}

	SPlayerState->SetPlayerReadyState(true);
}

bool ASPlayerController::Server_ReadyGame_Validate(bool bReadyState)
{
	return true;
}

void ASPlayerController::Client_SetInputMode_Implementation(bool bGameMode)
{
	if (true == bGameMode)
	{
		FInputModeGameAndUI GameInputMode;
		SetInputMode(GameInputMode);
	}
	else
	{
		FInputModeUIOnly UIInputMode;
		SetInputMode(UIInputMode);
	}
}