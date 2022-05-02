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
	SessionRoomPlayerController->SetPlayerName(FString::Printf(TEXT("Player%d"), PlayerControllerList.Num()));
	PlayerControllerList.Add(SessionRoomPlayerController);

	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr == SGameInstance)
	{
		return;
	}
	
	// TEST CODE : PostLogin()���� ���ǿ� ���� ���� ������ �ο� ���� ������ �� �ִ��� Ȯ��
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
		UE_LOG(LogTemp, Log, TEXT("Player %s Login"), *SPlayerController->GetPlayerName());
		Engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Player %s Login"), *SPlayerController->GetPlayerName()));
	}

	// TODO : �� ���� ó��
}

void ATestSessionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ASessionRoomPlayerController* SessionRoomPlayerController = Cast<ASessionRoomPlayerController>(Exiting);
	if (nullptr != SessionRoomPlayerController)
	{
		PlayerControllerList.Remove(SessionRoomPlayerController);
		UpdatePlayerList();
	}
}

void ATestSessionGameMode::StartGame()
{
	// TEST CODE
	GetWorld()->ServerTravel("/Game/Levels/TestBossLevel?listen");
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

		PlayerInfoList.Add(NewPlayerInfo);
		Index++;
	}

	for (auto SessionRoomPlayerController : PlayerControllerList)
	{
		// ��� �÷��̾��� PlayerList ���� ������Ʈ
		SessionRoomPlayerController->Client_UpdatePlayerList(PlayerInfoList);
	}
}

