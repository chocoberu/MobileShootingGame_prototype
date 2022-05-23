// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameState.h"
#include "Net/UnrealNetwork.h"

ATeamNormalGameState::ATeamNormalGameState()
{

}

void ATeamNormalGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamNormalGameState, CurrentGameTime);
}