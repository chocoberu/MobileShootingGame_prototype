// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameMode.h"
#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ATeamNormalGameMode::ATeamNormalGameMode()
{
	PlayerStateClass = ASPlayerState::StaticClass();
}

void ATeamNormalGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("TeamNormalGameMode Player %s Login"), *NewPlayer->GetName());
}

void ATeamNormalGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

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
		UE_LOG(LogTemp, Log, TEXT("Player Start : %s"), *Start->GetName());

		if (nullptr != PlayerState && PlayerState->GetPlayerName().Compare(Start->PlayerStartTag.ToString()) == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Find Player Start : %s"), *Start->PlayerStartTag.ToString());
			return Iter;
		}
	}

	// 해당 PlayerStart가 없는 경우 기존 방식으로 선택
	return Super::ChoosePlayerStart_Implementation(Player);
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