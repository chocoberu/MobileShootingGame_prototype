// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionRoomPlayerController.h"
#include "SPlayerState.h"
#include "TestSessionGameMode.h"
#include "SGameInstance.h"
#include "UI/TestSessionRoomWidget.h"
#include "Blueprint/UserWidget.h"

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

	// 비어있는 Row 초기화
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
		// 서버 측에 요청 필요
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
	
	bool bReady = IsPlayerReady();
	ReadyGame(!bReady);
}

void ASessionRoomPlayerController::ReadyGame(bool bReadyState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		// PlayerState에 값을 저장
		ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
		if (nullptr != SPlayerState)
		{
			SPlayerState->SetPlayerReadyState(bReadyState);
		}

		// 이후 플레이어 리스트 업데이트
		RequestServerPlayerList();
		Client_ReadyGame(bReadyState);
	}
	else
	{
		// 서버 측에 요청 필요
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
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr != SPlayerState)
	{
		SPlayerState->SetPlayerReadyState(bReadyState);
	}

	// TODO : Ready State == true인 경우 Weapon Select Button 비활성화
	// TODO : Ready State == true인 경우 PlayerState 값을 저장
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
			// TODO : SessionHost가 나갈 때 다른 클라이언트 kick 처리
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

void ASessionRoomPlayerController::Client_LeaveSession_Implementation()
{
	USGameInstance* SGameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance());
	if (nullptr != SGameInstance)
	{
		SGameInstance->LeaveAndDestroySession();
	}
}

void ASessionRoomPlayerController::SetPlayerName(const FString NewName)
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return;
	}

	SPlayerState->SetPlayerName(NewName);
}

FString ASessionRoomPlayerController::GetPlayerName()
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return FString();
	}

	return SPlayerState->GetPlayerName();
}

bool ASessionRoomPlayerController::IsPlayerReady() const
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return false;
	}

	return SPlayerState->IsPlayerReady();
}