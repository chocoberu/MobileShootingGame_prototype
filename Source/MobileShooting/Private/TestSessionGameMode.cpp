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
	
	// PlayerName을 Player0, Player1 과 같은 방식으로 설정
	// TODO : PlayerCount 값을 따로 만들어서 관리 필요 (현재 방식은 같은 이름이 나올 수 있음)
	// -> 해당 방식이 스레드 안전한가? 확인 필요
	SessionRoomPlayerController->SetPlayerName(FString::Printf(TEXT("Player%d"), PlayerControllerList.Num()));
	PlayerControllerList.Add(SessionRoomPlayerController);

	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr == SGameInstance)
	{
		return;
	}
	
	// TEST CODE : PostLogin()에서 세션에 현재 접속 가능한 인원 수를 차감할 수 있는지 확인
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

	// TODO : 팀 설정 처리
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

		// PlayerState의 정보를 이용해서 설정
		NewPlayerInfo.bPlayerReady = SPlayerState->IsPlayerReady();
		NewPlayerInfo.PlayerName = SPlayerState->GetPlayerName();

		PlayerInfoList.Add(NewPlayerInfo);
		Index++;
	}

	for (auto SessionRoomPlayerController : PlayerControllerList)
	{
		// 모든 플레이어의 PlayerList 위젯 업데이트
		SessionRoomPlayerController->Client_UpdatePlayerList(PlayerInfoList);
	}
}

