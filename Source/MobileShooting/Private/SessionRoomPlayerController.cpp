// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionRoomPlayerController.h"
#include "SPlayerState.h"
#include "TestSessionGameMode.h"
#include "SGameInstance.h"
#include "TestSaveGame.h"
#include "UI/TestSessionRoomWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASessionRoomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	if (false == IsLocalPlayerController() || nullptr == SessionRoomUIClass)
	{
		return;
	}

	SessionRoomWidget = CreateWidget<UTestSessionRoomWidget>(this, SessionRoomUIClass);
	if (nullptr == SessionRoomWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SessionRoom Widget is nullptr"));
		return;
	}

	if (GetLocalRole() != ROLE_Authority)
	{
		SessionRoomWidget->SetStartButtonVisible(false);
	}
	SessionRoomWidget->AddToViewport();

	SPlayerState = GetPlayerState<ASPlayerState>();
	if(nullptr == SPlayerState)
	{
		UE_LOG(LogTemp, Warning, TEXT("SPlayerState is nullptr"));
	}
	
	RequestServerPlayerList();
}

void ASessionRoomPlayerController::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	UE_LOG(LogTemp, Log, TEXT("ASessionRoomPlayerController::EndPlay() called, PlayerController : %s"), *GetName());	
}

void ASessionRoomPlayerController::UpdatePlayerList(const TArray<FRoomPlayerInfo>& PlayerInfoList)
{
	int32 Index = 0;
	for (const FRoomPlayerInfo& PlayerInfo : PlayerInfoList)
	{
		FString PlayerName;
		if (0 == PlayerInfo.PlayerName.Compare(GetPlayerName()))
		{
			PlayerName = TEXT("ME");
		}
		else
		{
			PlayerName = PlayerInfo.PlayerName;
		}

		SessionRoomWidget->SetPlayerRowByIndex(Index, PlayerName, PlayerInfo.bPlayerReady);
		Index++;
	}

	// ����ִ� Row �ʱ�ȭ
	// �ϵ� �ڵ� ���� �ʿ�
	for (; Index < 4; Index++)
	{
		SessionRoomWidget->SetPlayerRowByIndex(Index, TEXT("Empty"), false);
	}
}

void ASessionRoomPlayerController::RequestServerPlayerList()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ATestSessionGameMode* SessionGameMode = Cast<ATestSessionGameMode>(GetWorld()->GetAuthGameMode());
		if (nullptr != SessionGameMode)
		{
			SessionGameMode->UpdatePlayerList();
		}
	}
	else
	{
		// ���� ���� ��û �ʿ�
		Server_RequestServerPlayerList();
	}
}

void ASessionRoomPlayerController::Server_RequestServerPlayerList_Implementation()
{
	RequestServerPlayerList();
}

bool ASessionRoomPlayerController::Server_RequestServerPlayerList_Validate()
{
	return true;
}

void ASessionRoomPlayerController::ChangeReadyState()
{
	UE_LOG(LogTemp, Log, TEXT("Change Ready State"));
	
	if (nullptr == SPlayerState)
	{
		return;
	}

	bool bReady = IsPlayerReady();

	if (false == bReady)
	{
		UTestSaveGame* NewSaveGame = NewObject<UTestSaveGame>();
		NewSaveGame->MainWeaponId = SPlayerState->GetWeaponId();
		NewSaveGame->SubWeaponId = SPlayerState->GetSubWeaponId();

		if (false == UGameplayStatics::SaveGameToSlot(NewSaveGame, SPlayerState->GetPlayerName(), 0))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save data!"));
			return;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Save Success"));
		}
	}

	ReadyGame(!bReady);
}

void ASessionRoomPlayerController::ReadyGame(bool bReadyState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		// PlayerState�� ���� ����
		if (nullptr != SPlayerState)
		{
			SPlayerState->SetPlayerReadyState(bReadyState);
		}

		// ���� �÷��̾� ����Ʈ ������Ʈ
		RequestServerPlayerList();
		Client_ReadyGame(bReadyState);
	}
	else
	{
		// ���� ���� ��û �ʿ�
		Server_ReadyGame(bReadyState);
	}
}

void ASessionRoomPlayerController::Server_ReadyGame_Implementation(bool bReadyState)
{
	ReadyGame(bReadyState);
}

bool ASessionRoomPlayerController::Server_ReadyGame_Validate(bool bReadyState)
{
	return true;
}

void ASessionRoomPlayerController::Client_ReadyGame_Implementation(bool bReadyState)
{
	if (nullptr == SPlayerState)
	{
		return;
	}
	SPlayerState->SetPlayerReadyState(bReadyState);

	// TODO : Ready State == true�� ��� Weapon Select Button ��Ȱ��ȭ
	
	
}

void ASessionRoomPlayerController::StartGame()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ATestSessionGameMode* SessionGameMode = Cast<ATestSessionGameMode>(GetWorld()->GetAuthGameMode());
		if (nullptr != SessionGameMode)
		{
			SessionGameMode->StartGame();
		}
	}
}

void ASessionRoomPlayerController::Client_UpdatePlayerList_Implementation(const TArray<FRoomPlayerInfo>& PlayerInfoList)
{
	UpdatePlayerList(PlayerInfoList);
}

void ASessionRoomPlayerController::LeaveSession(bool bSessionHost)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ATestSessionGameMode* SessionGameMode = Cast<ATestSessionGameMode>(GetWorld()->GetAuthGameMode());
		if (nullptr != SessionGameMode)
		{
			// TODO : SessionHost�� ���� �� �ٸ� Ŭ���̾�Ʈ kick ó��
			SessionGameMode->LeaveSession(GetPlayerName(), bSessionHost);
		}
	}
	else
	{
		Server_LeaveSession();
	}
}

void ASessionRoomPlayerController::Server_LeaveSession_Implementation()
{
	LeaveSession(false);
}

bool ASessionRoomPlayerController::Server_LeaveSession_Validate()
{
	return true;
}

void ASessionRoomPlayerController::Client_LeaveSession_Implementation(bool bKicked)
{
	// TODO : bKicked�� ��� ErrorMessage ó�� �ʿ�
	USGameInstance* SGameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance());
	if (nullptr != SGameInstance)
	{
		if (true == bKicked)
		{
			SGameInstance->ShowErrorMessage(0);
		}
		SGameInstance->LeaveAndDestroySession();
	}
	
}

void ASessionRoomPlayerController::SetPlayerName(const FString NewName)
{
	if (nullptr == SPlayerState)
	{
		return;
	}

	SPlayerState->SetPlayerName(NewName);
}

FString ASessionRoomPlayerController::GetPlayerName()
{
	if (nullptr == SPlayerState)
	{
		return FString();
	}

	return SPlayerState->GetPlayerName();
}

bool ASessionRoomPlayerController::IsPlayerReady() const
{
	if (nullptr == SPlayerState)
	{
		return false;
	}

	return SPlayerState->IsPlayerReady();
}

void ASessionRoomPlayerController::SetWeaponId(const int32 WeaponId)
{
	if (nullptr == SPlayerState)
	{
		return;
	}
	SPlayerState->SetWeaponId(WeaponId);
}

void ASessionRoomPlayerController::SetSubWeaponId(const int32 SubWeaponId)
{
	if (nullptr == SPlayerState)
	{
		return;
	}
	SPlayerState->SetSubWeaponId(SubWeaponId);
}