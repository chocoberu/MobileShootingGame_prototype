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


	PlayerCount = 0;
}

void ATestSessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ASessionRoomPlayerController* SessionRoomPlayerController = Cast<ASessionRoomPlayerController>(NewPlayer);
	if (nullptr == SessionRoomPlayerController)
	{
		return;
	}

	// PlayerName�� Player0, Player1 �� ���� ������� ����
	// TODO : PlayerCount ���� ���� ���� ���� �ʿ� (���� ����� ���� �̸��� ���� �� ����)
	// -> �ش� ����� ������ �����Ѱ�? Ȯ�� �ʿ�
	SessionRoomPlayerController->SetPlayerName(FString::Printf(TEXT("Player%d"), PlayerCount++));

	// �� ���� ó��
	ASPlayerState* SPlayerState = SessionRoomPlayerController->GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return;
	}
	SPlayerState->SetTeamNumber(PlayerControllerList.Num() % 2);
	PlayerControllerList.Add(SessionRoomPlayerController);

	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr == SGameInstance)
	{
		return;
	}

	// TEST CODE : PostLogin()���� ���ǿ� ���� ���� ������ �ο� ���� ������ �� �ִ��� Ȯ��
	// Logout() �ɶ��� ó�� �ʿ�
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (nullptr != Subsystem)
	{
		auto SessionInterface = Subsystem->GetSessionInterface();
		if (true == SessionInterface.IsValid())
		{
			auto ExitingSession = SessionInterface->GetNamedSession(TEXT("Test Session"));
			if (nullptr != ExitingSession)
			{
				ExitingSession->NumOpenPublicConnections--;
			}
		}
	}

	auto Engine = SGameInstance->GetEngine();
	if (nullptr == Engine)
	{
		return;
	}

	for (auto SPlayerController : PlayerControllerList)
	{
		UE_LOG(LogTemp, Log, TEXT("%s Login"), *SPlayerController->GetPlayerName());
		Engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Player %s Login"), *SPlayerController->GetPlayerName()));
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
	PlayerControllerList.Remove(SessionRoomPlayerController);
	UpdatePlayerList();

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (nullptr != Subsystem)
	{
		auto SessionInterface = Subsystem->GetSessionInterface();
		if (true == SessionInterface.IsValid())
		{
			auto ExitingSession = SessionInterface->GetNamedSession(TEXT("Test Session"));
			if (nullptr != ExitingSession)
			{
				ExitingSession->NumOpenPublicConnections++;
			}
		}
	}
}

void ATestSessionGameMode::StartGame()
{
	// Player ��ΰ� Ready �������� Ȯ��
	bool bCanStartGame = true;
	for (auto SessionRoomPlayerController : PlayerControllerList)
	{
		if (false == SessionRoomPlayerController->IsPlayerReady())
		{
			bCanStartGame = false;
			break;
		}
	}

	if (false == bCanStartGame)
	{
		// TODO : ���� Widget ���̵��� ���� (������ ���̵���)
		UE_LOG(LogTemp, Log, TEXT("Can't Start Game, All Players must be ready state"));
		return;
	}

	// Session Start
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr == SGameInstance)
	{
		return;
	}
	SGameInstance->StartSession();
}

void ATestSessionGameMode::UpdatePlayerList()
{
	PlayerInfoList.Empty();

	int32 Index = 0;
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
		NewPlayerInfo.TeamNumber = Index % 2;
		SPlayerState->SetTeamNumber(NewPlayerInfo.TeamNumber);

		PlayerInfoList.Add(NewPlayerInfo);
		Index++;

		UE_LOG(LogTemp, Log, TEXT("PlayerList Add(), %s, %d"), *NewPlayerInfo.PlayerName, NewPlayerInfo.bPlayerReady);
	}

	for (auto SessionRoomPlayerController : PlayerControllerList)
	{
		// ��� �÷��̾��� PlayerList ���� ������Ʈ
		SessionRoomPlayerController->Client_UpdatePlayerList(PlayerInfoList);
	}
}

void ATestSessionGameMode::LeaveSession(const FString PlayerName)
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