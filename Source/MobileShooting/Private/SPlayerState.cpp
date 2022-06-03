// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "TestSaveGame.h"
#include "SGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "Net/OnlineEngineInterface.h"

ASPlayerState::ASPlayerState()
{
	ResetPlayerState();

	// TEST CODE : 하드코딩, 수정 필요
	SetPlayerName(TEXT("Player0"));
	CurrentWeaponId = 0;
	CurrentSubweaponId = 1000;

	SessionName = TEXT("Test Session");
	bNetLoadOnClient = true;
}

void ASPlayerState::RegisterPlayerWithSession(bool bWasFromInvite)
{
	if (GetNetMode() != NM_Standalone)
	{
		if (true == GetUniqueId().IsValid())
		{
			USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
			if (nullptr != SGameInstance)
			{
				SGameInstance->RegisterPlayer(SessionName, *GetUniqueId(), bWasFromInvite);
			}
		}
	}
}

void ASPlayerState::UnregisterPlayerWithSession()
{
	if (GetNetMode() == NM_Client && true == GetUniqueId().IsValid())
	{
		USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
		if (SessionName != NAME_None && nullptr != SGameInstance)
		{
			UE_LOG(LogTemp, Log, TEXT("ASPlayerState::UnregisterPlayerWithSession(), "));
			SGameInstance->UnregisterPlayer(SessionName, *GetUniqueId());
		}
	}
}

void ASPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("ASPlayerState::BeginPlay() call"));
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
	DOREPLIFETIME(ASPlayerState, KillScore);
	DOREPLIFETIME(ASPlayerState, DeathScore);
	DOREPLIFETIME(ASPlayerState, AssistScore);
	
}