// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameMode.h"
#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SGameSession.h"
#include "SGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ATeamNormalGameMode::ATeamNormalGameMode()
{
	PlayerStateClass = ASPlayerState::StaticClass();
	GameSessionClass = ASGameSession::StaticClass();

	bDelayedStart = true;
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
	
	// TODO : 로그인한 플레이어 수가 세션의 플레이어 수와 일치하면 StartMatch()
	// TEST CODE
	if (PlayerControllerList.Num() == 1)
	{
		StartMatch();
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

	if (true == ReadyToStartMatch())
	{
		UE_LOG(LogTemp, Log, TEXT("ATeamNormalGameMode::StartMatch(), Ready to Start Match"));
	}
}

void ATeamNormalGameMode::EndMatch()
{
	Super::EndMatch();

	UE_LOG(LogTemp, Log, TEXT("ATeamNormalGameMode::EndMatch() called"));
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