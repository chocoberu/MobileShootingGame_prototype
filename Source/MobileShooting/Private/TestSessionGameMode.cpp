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

	BlueTeamCount = RedTeamCount = 0;
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
	int32 PlayerIndex = BlueTeamCount <= RedTeamCount ? 0 : 1;
	for (; PlayerIndex < 4; PlayerIndex += 2)
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

		// 팀 설정
		ASPlayerState* SPlayerState = SessionRoomPlayerController->GetPlayerState<ASPlayerState>();
		if (nullptr == SPlayerState)
		{
			UE_LOG(LogTemp, Error, TEXT("ATestSessionGameMode::PostLogin(), SPlayerState is nullptr"));
			return;
		}
		SPlayerState->SetTeamNumber(PlayerIndex % 2);
		if (PlayerIndex % 2 == 0)
		{
			BlueTeamCount++;
		}
		else
		{
			RedTeamCount++;
		}
		break;
	}
	PlayerControllerList.Add(SessionRoomPlayerController);

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
		UE_LOG(LogTemp, Log, TEXT("%s Login"), *SPlayerController->GetPlayerName());
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
	
	ASPlayerState* SPlayerState = SessionRoomPlayerController->GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return;
	}
	if (0 == SPlayerState->GetTeamNumber() % 2)
	{
		BlueTeamCount--;
	}
	else
	{
		RedTeamCount--;
	}

	PlayerControllerList.Remove(SessionRoomPlayerController);
	UpdatePlayerList();

	// TEST CODE : Logout()에서 세션에 현재 접속 가능한 인원 수를 차감할 수 있는지 확인, 수정 필요
	/*IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
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
	}*/
}

bool ATestSessionGameMode::StartGame()
{
	// Player 모두가 Ready 상태인지 확인
	// TODO : Host의 경우 Ready 상태인지 확인 안해도 될거 같음

	// 홀수 인원인 경우 게임을 시작할 수 없음
	// TEST : 테스트를 위해 임시 주석 처리
	/*if (0 != PlayerControllerList.Num() % 2 || BlueTeamCount != RedTeamCount)
	{
		return false;
	}*/

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
		// TODO : 오류 Widget 보이도록 수정 (서버만 보이도록)
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
	BlueTeamCount = RedTeamCount = 0;

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
		NewPlayerInfo.TeamNumber = SPlayerState->GetTeamNumber();
		
		PlayerInfoList.Add(NewPlayerInfo);

		UE_LOG(LogTemp, Log, TEXT("PlayerList Add(), %s, %d"), *NewPlayerInfo.PlayerName, NewPlayerInfo.bPlayerReady);
	}

	for (auto SessionRoomPlayerController : PlayerControllerList)
	{
		// 모든 플레이어의 PlayerList 위젯 업데이트
		SessionRoomPlayerController->Client_UpdatePlayerList(PlayerInfoList);
	}
}

void ATestSessionGameMode::LeaveSession(const FString PlayerName, bool bSessionHost)
{
	// 호스트가 떠나는 경우 클라이언트도 LeaveSession() 처리
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

	// 호스트가 아닌 경우
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