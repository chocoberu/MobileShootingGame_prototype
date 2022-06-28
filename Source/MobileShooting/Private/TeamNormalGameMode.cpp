// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameMode.h"
#include "TeamNormalGameState.h"
#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SGameSession.h"
#include "SGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ATeamNormalGameMode::ATeamNormalGameMode()
{
	PlayerStateClass = ASPlayerState::StaticClass();
	GameStateClass = ATeamNormalGameState::StaticClass();
	GameSessionClass = ASGameSession::StaticClass();

	bDelayedStart = true;
	StartCount = 4;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.16f;
}

void ATeamNormalGameMode::InitGameState()
{
	Super::InitGameState();

	ATeamNormalGameState* TeamNormalGameState = GetGameState<ATeamNormalGameState>();
	if (nullptr == TeamNormalGameState)
	{
		UE_LOG(LogTemp, Error, TEXT("ATeamNormalGameMode::InitGameState(), TeamNormalGameState is nullptr"));
		return;
	}

	TeamNormalGameState->OnAllPlayerReadyDelegate.AddUObject(this, &ATeamNormalGameMode::CountForStartMatch);
}

void ATeamNormalGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("TeamNormalGameMode Player %s PostLogin"), *NewPlayer->GetName());

	ASPlayerController* SPlyaerController = Cast<ASPlayerController>(NewPlayer);
	if (nullptr != SPlyaerController)
	{
		PlayerControllerList.Add(SPlyaerController);
	}

	ATeamNormalGameState* TeamNormalGameState = GetGameState<ATeamNormalGameState>();
	if (nullptr != TeamNormalGameState)
	{
		TeamNormalGameState->AddPlayerState(NewPlayer->GetPlayerState<APlayerState>());
	}

	int32 CurrentPlayerCount = 1;
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr != SGameInstance)
	{
		CurrentPlayerCount = SGameInstance->GetCurrentSessionPlayerCount();
	}
	
	UE_LOG(LogTemp, Log, TEXT("Total Player Count : %d"), CurrentPlayerCount);

	// TODO : 모든 플레이어가 준비 되었을 때 Match Start
	// TEST : 임시로 조건 추가
	if (PlayerControllerList.Num() == CurrentPlayerCount || -1 == CurrentPlayerCount)
	{
		SetMatchState(MatchState::InProgress); // 테스트용
		//CountForStartMatch();
	}
}

void ATeamNormalGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ASPlayerController* SPlyaerController = Cast<ASPlayerController>(Exiting);
	if (nullptr != SPlyaerController)
	{
		PlayerControllerList.Remove(SPlyaerController);
	}

	UE_LOG(LogTemp, Log, TEXT("TeamNormalGameMode Logout : %s"), *Exiting->GetName());
}

void ATeamNormalGameMode::RestartPlayer(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("TeamNormalGameMode Restart Player : %s"), *NewPlayer->GetName());

	ASPlayerController* SPlayerController = Cast<ASPlayerController>(NewPlayer);
	if (nullptr == SPlayerController)
	{
		Super::RestartPlayer(NewPlayer);
		return;
	}
	
	// ASPlayerController에서 PlayerState 정보를 읽어옴
	SPlayerController->RequestRestartPlayerDelegate.AddUObject(this, &ATeamNormalGameMode::ResponseRestartPlayer);
	SPlayerController->Client_LoadPlayerStateInfo();
}

AActor* ATeamNormalGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogTemp, Log, TEXT("ATeamNormalGameMode::ChoosePlayerStart() called"));

	ASPlayerController* PlayerController = Cast<ASPlayerController>(Player);
	if (nullptr == PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("Player %s is not SPlayerController"), *Player->GetName());
	}
	ASPlayerState* PlayerState = PlayerController->GetPlayerState<ASPlayerState>();
	if (nullptr == PlayerState)
	{
		UE_LOG(LogTemp, Log, TEXT("Player %s's PlayerState is nullptr"), *Player->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerName : %s"), *PlayerState->GetPlayerName());
	}

	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr == SGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("SGameInstance is nullptr"));
	}

	// PlayerStart 중에서 PlayerStartTag가 일치하는 것을 선택
	TArray<AActor*> Results;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Results);

	for (auto Iter : Results)
	{
		APlayerStart* Start = Cast<APlayerStart>(Iter);
		if (nullptr == Iter)
		{
			continue;
		}
		
		if (nullptr != PlayerState && PlayerState->GetPlayerName().Compare(Start->PlayerStartTag.ToString()) == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Find Player Start : %s"), *Start->PlayerStartTag.ToString());
			Player->StartSpot = Iter; // StartSpot에 저장
			return Iter;
		}
	}

	// 해당 PlayerStart가 없는 경우 기존 방식으로 선택
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ATeamNormalGameMode::StartMatch()
{
	Super::StartMatch();

	UE_LOG(LogTemp, Log, TEXT("ATeamNormalGameMode::StartMatch() called"));

	ATeamNormalGameState* TeamNormalGameState = GetGameState<ATeamNormalGameState>();
	if (nullptr != TeamNormalGameState)
	{
		TeamNormalGameState->SetStartGameTime();
	}

	// TODO : 모든 플레이어 움직이도록 처리
	for (auto Player : PlayerControllerList)
	{
		Player->Client_SetInputMode(true);
	}
}

void ATeamNormalGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MatchState::InProgress == GetMatchState())
	{
		ATeamNormalGameState* TeamNormalGameState = GetGameState<ATeamNormalGameState>();
		if (nullptr == TeamNormalGameState)
		{
			return;
		}

		if (TeamNormalGameState->GetCurrentGamePlayTime() <= 0.0f)
		{
			UE_LOG(LogTemp, Log, TEXT("Game Over!, Match end"));
			EndMatch();
		}
	}
}

void ATeamNormalGameMode::CountForStartMatch()
{
	StartCount--;
	UE_LOG(LogTemp, Log, TEXT("Start Count : %d"), StartCount);

	// TEST CODE
	if (GetMatchState() != MatchState::InProgress)
	{
		SetMatchState(MatchState::InProgress);
	}

	ATeamNormalGameState* TeamNormalGameState = GetGameState<ATeamNormalGameState>();
	if (nullptr != TeamNormalGameState)
	{
		TeamNormalGameState->Multicast_CountDown(StartCount);
	}


	if (0 < StartCount)
	{
		GetWorldTimerManager().SetTimer(StartTimer, this, &ATeamNormalGameMode::CountForStartMatch, 1.0f, false);
	}
	else
	{
		StartMatch();
	}
}

void ATeamNormalGameMode::EndMatch()
{
	Super::EndMatch();

	UE_LOG(LogTemp, Log, TEXT("ATeamNormalGameMode::EndMatch() called"));

	// TODO : 모든 플레이어 멈추도록 처리
	for (auto Player : PlayerControllerList)
	{
		Player->Client_SetInputMode(false);
	}
}

void ATeamNormalGameMode::ResponseRestartPlayer(AController* NewPlayer)
{
	UE_LOG(LogTemp, Log, TEXT("ATeamNormalGameMode::ResponseRestartPlayer() called"));

	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	AActor* StartSpot = ChoosePlayerStart(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("ResponseRestartPlayer(), StartSpot : %s"), *StartSpot->GetName());

	// AGameModeBase의 코드 사용
	// If a start spot wasn't found,
	if (StartSpot == nullptr)
	{
		// Check for a previously assigned spot
		if (NewPlayer->StartSpot != nullptr)
		{
			StartSpot = NewPlayer->StartSpot.Get();
			UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
		}
	}

	RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}