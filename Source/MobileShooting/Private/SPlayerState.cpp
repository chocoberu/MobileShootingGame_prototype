// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "TestSaveGame.h"
#include "Net/UnrealNetwork.h"

ASPlayerState::ASPlayerState()
{
	ResetPlayerState();

	// TEST CODE : �ϵ��ڵ�, ���� �ʿ�
	CurrentWeaponId = 0;
	CurrentSubweaponId = 1000;
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

void ASPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, TeamNumber);
	//DOREPLIFETIME(ASPlayerState, bPlayerReady);
}