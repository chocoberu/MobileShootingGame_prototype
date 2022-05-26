// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameState.h"
#include "TeamNormalGameMode.h"
#include "Net/UnrealNetwork.h"

ATeamNormalGameState::ATeamNormalGameState()
{
	GameModeClass = ATeamNormalGameMode::StaticClass();
}

void ATeamNormalGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamNormalGameState, CurrentGameTime);
}