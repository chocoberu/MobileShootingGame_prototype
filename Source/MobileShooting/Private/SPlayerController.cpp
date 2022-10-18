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

	// PlayerState ������ �ҷ����� ����
	if (false == bLoadPlayerState)
	{
		Client_LoadPlayerStateInfo();
	}
}

void ASPlayerController::BindMainWeaponStatusWidget(ASWeapon* MainWeapon)
{
	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BindMainWeaponStatusWidget() called"));
	if (false == IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BindMainWeaponStatusWidget(), Is not local player"));
		return;
	}

	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("ASPlayerController::BindMainWeaponStatusWidget(), RightButtonHUD is nullptr"));
		return;
	}
	RightButtonHUD->BindMainWeapon(MainWeapon);
}

void ASPlayerController::BindSubWeaponStatusWidget(ASSubWeapon* SubWeapon)
{
	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BindSubWeaponStatusWidget() called"));
	if (false == IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BindSubWeaponStatusWidget(), Is not local player"));
		return;
	}

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

	// Input Mode�� UI�� ����
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

	// Input Mode�� GameAndUI�� ����
	FInputModeGameAndUI Mode;
	SetInputMode(Mode);
}

void ASPlayerController::OnPlayerDead()
{
	// ���� �е��� ��� ���� �ִϸ��̼��� ����
	if (nullptr == RightButtonHUD)
	{
		return;
	}
	RightButtonHUD->StopAllWidgetAnimations();
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

	if (false == IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BeginPlay(), No Local Controller"));
		return;
	}

	// ��Ƽ �÷����� ��� GameMode���� SetInputMode�� �ٲٵ��� ����
	if (GetNetMode() != ENetMode::NM_Standalone)
	{
		FInputModeUIOnly UIInputMode;
		//SetInputMode(UIInputMode);
	}
	
	FString CurrentPlatform = UGameplayStatics::GetPlatformName();
	// �ӽ� �ּ� ó��
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

void ASPlayerController::SetPlayerStateInfo(const FString& NewPlayerName, int32 NewWeaponId, int32 NewSubWeaponId, int32 NewPlayerIndex)
{
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

	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::SetPlayerStateInfo(), after RequestRestartPlayerDelegate called"));
	ASCharacter* PlayerCharacter = Cast<ASCharacter>(GetPawn());
	if (nullptr == PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("ASPlayerController::SetPlayerStateInfo(), PlayerCharacter is nullptr"));
		return;
	}

	PlayerCharacter->SetGenericTeamId(SPlayerState->GetTeamNumber());
	PlayerCharacter->LoadWeapon();

}

void ASPlayerController::Client_LoadPlayerStateInfo_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::Client_LoadPlayerStateInfo() called"));

	// BeginPlay()���� InitWidget() ȣ�� �� �Ϻ� �÷������� HUD�� Weapon Bind�� ����� ���� ���� (HUD == nullptr�̿���)
	// PostInitializeComponents()������ Possess �����̱� ������ CreateWidget �Ұ�
	// OnPossess()�� Ŭ���̾�Ʈ���� ȣ����� �����Ƿ� �ٸ� ������ ���� �ʿ�
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
			SetPlayerStateInfo(PlayerName, SaveGame->MainWeaponId, SaveGame->SubWeaponId, SaveGame->PlayerIndex);
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

	SetPlayerStateInfo(NewPlayerName, NewWeaponId, NewSubWeaponId, NewPlayerIndex);
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

	SPlayerState->SetPlayerReadyState(bReadyState);
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