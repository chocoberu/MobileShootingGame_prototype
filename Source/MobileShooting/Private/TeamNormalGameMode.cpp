// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameMode.h"

ATeamNormalGameMode::ATeamNormalGameMode()
{

}

void ATeamNormalGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("Player %s Login"), *NewPlayer->GetName());
}

void ATeamNormalGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ATeamNormalGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("Restart Player : %s"), *NewPlayer->GetName());
}

AActor* ATeamNormalGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// TODO : 해당 부분 구현 필요
	return Super::ChoosePlayerStart_Implementation(Player);
}
