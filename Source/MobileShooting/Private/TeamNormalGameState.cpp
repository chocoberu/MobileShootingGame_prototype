// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameState.h"
#include "TeamNormalGameMode.h"
#include "Net/UnrealNetwork.h"

ATeamNormalGameState::ATeamNormalGameState()
{
	GameModeClass = ATeamNormalGameMode::StaticClass();

	CurrentGameTime = 500;
	PrimaryActorTick.bCanEverTick = true;
}

void ATeamNormalGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamNormalGameState, CurrentGameTime);
}

void ATeamNormalGameState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("ATeamNormalGameState::BeginPlay() called"));
}

void ATeamNormalGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//UE_LOG(LogTemp, Log, TEXT("Current MatchState : %s"), *GetMatchState().ToString());

	if (true == HasMatchStarted())
	{
		UE_LOG(LogTemp, Log, TEXT("GetServerWorldTimeSeconds : %f"), GetServerWorldTimeSeconds());
	}
}
