// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSessionGameMode.h"
#include "SGameInstance.h"
#include "SessionRoomPlayerController.h"
#include "SPlayerState.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

ATestSessionGameMode::ATestSessionGameMode()
{
	PlayerControllerClass = ASessionRoomPlayerController::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();

	BlueTeamCount = RedTeamCount = 0;
	MaxPlayerCount = 4;
}

void ATestSessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ASessionRoomPlayerController* SessionRoomPlayerController = Cast<ASessionRoomPlayerController>(NewPlayer);
	if (nullptr == SessionRoomPlayerController)
	{
		return;
	}

	// PlayerName�� Player0, Player1... PlayerN �� ���� ������� ����
	int32 PlayerIndex = BlueTeamCount <= RedTeamCount ? 0 : 1;
	for (; PlayerIndex < MaxPlayerCount; PlayerIndex += 2)
	{
		FString TempPlayerName = FString::Printf(TEXT("Player%d"), PlayerIndex);
		bool bExist = false;
		for (auto PC : PlayerControllerList)
		{
			if (0 == PC->GetPlayerName().Compare(TempPlayerName))
			{
				bExist = true;
				break;
			}
		}
		if (true == bExist)
		{
			continue;
		}

		SessionRoomPlayerController->SetPlayerName(TempPlayerName);
		break;
	}
	
	// �� ����
	ASPlayerState* SPlayerState = SessionRoomPlayerController->GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("ATestSessionGameMode::PostLogin(), SPlayerState is nullptr"));
		return;
	}
	SPlayerState->SetPlayerIndex(PlayerIndex);
	if (PlayerIndex % 2 == 0)
	{
		BlueTeamCount++;
	}
	else
	{
		RedTeamCount++;
	}

	PlayerControllerList.Add(SessionRoomPlayerController);

	// Log ���� ó��
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr == SGameInstance)
	{
		return;
	}
	auto Engine = SGameInstance->GetEngine();
	if (nullptr == Engine)
	{
		return;
	}

	for (auto SPlayerController : PlayerControllerList)
	{
		UE_LOG(LogTemp, Log, TEXT("ATestSessionGameMode::PostLogin(), %s Login"), *SPlayerController->GetPlayerName());
		Engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("%s Login"), *SPlayerController->GetPlayerName()));
	}
}

void ATestSessionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ASessionRoomPlayerController* SessionRoomPlayerController = Cast<ASessionRoomPlayerController>(Exiting);
	if (nullptr == SessionRoomPlayerController)
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("ATestSessionGameMode::Logout() called, PlayerController : %s"), *SessionRoomPlayerController->GetName());
	
	// �� ���� ó��
	ASPlayerState* SPlayerState = SessionRoomPlayerController->GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return;
	}
	if (0 == SPlayerState->GetTeamNumber())
	{
		BlueTeamCount--;
	}
	else
	{
		RedTeamCount--;
	}

	PlayerControllerList.Remove(SessionRoomPlayerController);
	UpdatePlayerList();
}

bool ATestSessionGameMode::StartGame()
{
	// Ȧ�� �ο��� ��� ������ ������ �� ����
	// TEST : �׽�Ʈ�� ���� �ӽ� �ּ� ó��
	/*if (0 != PlayerControllerList.Num() % 2 || BlueTeamCount != RedTeamCount)
	{
		return false;
	}*/

	// Player ��ΰ� Ready �������� Ȯ��
	bool bCanStartGame = true;
	for (auto SessionRoomPlayerController : PlayerControllerList)
	{
		if (false == SessionRoomPlayerController->IsPlayerReady() /*&& false == SessionRoomPlayerController->IsSessionHost()*/)
		{
			bCanStartGame = false;
			break;
		}
	}

	if (false == bCanStartGame)
	{
		// TODO : ���� Widget ���̵��� ���� (������ ���̵���)
		UE_LOG(LogTemp, Log, TEXT("Can't Start Game, All Players must be ready state"));
		return false;
	}

	// Session Start
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr == SGameInstance)
	{
		return false;
	}
	SGameInstance->StartSession();
	return true;
}

void ATestSessionGameMode::UpdatePlayerList()
{
	PlayerInfoList.Empty();
	
	for (auto SessionRoomPlayerController : PlayerControllerList)
	{
		FRoomPlayerInfo NewPlayerInfo;

		ASPlayerState* SPlayerState = SessionRoomPlayerController->GetPlayerState<ASPlayerState>();
		if (nullptr == SPlayerState)
		{
			continue;
		}

		// PlayerState�� ������ �̿��ؼ� ����
		NewPlayerInfo.bPlayerReady = SPlayerState->IsPlayerReady();
		NewPlayerInfo.PlayerName = SPlayerState->GetPlayerName();
		NewPlayerInfo.PlayerIndex = SPlayerState->GetPlayerIndex();
		
		PlayerInfoList.Add(NewPlayerInfo);

		UE_LOG(LogTemp, Log, TEXT("PlayerList Add(), %s, %d"), *NewPlayerInfo.PlayerName, NewPlayerInfo.bPlayerReady);
	}

	// ��� �÷��̾��� PlayerList ���� ������Ʈ
	for (auto SessionRoomPlayerController : PlayerControllerList)
	{
		SessionRoomPlayerController->Client_UpdatePlayerList(PlayerInfoList);
	}
}

void ATestSessionGameMode::LeaveSession(const FString& PlayerName, bool bSessionHost)
{
	// ȣ��Ʈ�� ������ ��� Session ���� ��� Ŭ���̾�Ʈ�� LeaveSession() ó��
	if (true == bSessionHost)
	{
		for (auto SessionRoomPlayerController : PlayerControllerList)
		{
			if (SessionRoomPlayerController->GetPlayerName().Compare(PlayerName) == 0)
			{
				SessionRoomPlayerController->Client_LeaveSession();
			}
			else
			{
				SessionRoomPlayerController->Client_LeaveSession(true);
			}
		}
	}

	// ȣ��Ʈ�� �ƴ� ���
	else
	{
		for (auto SessionRoomPlayerController : PlayerControllerList)
		{
			if (SessionRoomPlayerController->GetPlayerName().Compare(PlayerName) == 0)
			{
				SessionRoomPlayerController->Client_LeaveSession();
				return;
			}
		}
	}
}