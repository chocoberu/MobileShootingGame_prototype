// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameState.h"
#include "TeamNormalGameMode.h"
#include "Net/UnrealNetwork.h"

ATeamNormalGameState::ATeamNormalGameState()
{
	GameModeClass = ATeamNormalGameMode::StaticClass();

	PrimaryActorTick.bCanEverTick = true;

	GamePlayTime = 300.0f;
}

void ATeamNormalGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamNormalGameState, StartGameTime);
}

void ATeamNormalGameState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("ATeamNormalGameState::BeginPlay() called"));
}

void ATeamNormalGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (false == HasAuthority())
	{
		return;
	}

	if (true == HasMatchStarted())
	{
		//UE_LOG(LogTemp, Log, TEXT("GetServerWorldTimeSeconds : %f"), GetServerWorldTimeSeconds());
		UE_LOG(LogTemp, Log, TEXT("Current Time : %f"), GetServerWorldTimeSeconds() - StartGameTime);
	}

}

void ATeamNormalGameState::SetStartGameTime()
{
	UWorld* World = GetWorld();
	if (nullptr != World)
	{
		StartGameTime = World->GetTimeSeconds();
	}
}