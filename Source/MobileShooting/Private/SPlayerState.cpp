// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "TestSaveGame.h"

ASPlayerState::ASPlayerState()
{
	ResetPlayerState();
}

void ASPlayerState::ResetPlayerState()
{
	KillScore = DeathScore = AssistScore = 0;
}

void ASPlayerState::AddKillScore()
{
	++KillScore;
	UE_LOG(LogTemp, Log, TEXT("Current Kill Score : %d"), KillScore);
}

void ASPlayerState::AddDeathScore()
{
	++DeathScore;
	UE_LOG(LogTemp, Log, TEXT("Current Death Score : %d"), DeathScore);
}

void ASPlayerState::AddAssistScore()
{
	++AssistScore;
	UE_LOG(LogTemp, Log, TEXT("Current Assist Score : %d"), AssistScore);
}