// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASPlayerState();

	uint32 GetKillScore() const { return KillScore; }

	uint32 GetDeathScore() const { return DeathScore; }

	uint32 GetAssistScore() const { return AssistScore; }

	void ResetPlayerState();

	void AddKillScore();

	void AddDeathScore();

	void AddAssistScore();

	void SetPlayerReadyState(const bool NewFlag) { bPlayerReady = NewFlag; }

	bool IsPlayerReady() const { return bPlayerReady; }

	void SetTeamNumber(const int32 Team) { TeamNumber = Team; }

	int32 GetTeamNumber() const { return TeamNumber; }

protected:

	int32 TeamNumber;

	bool bPlayerReady;

	uint32 KillScore;

	uint32 DeathScore;

	uint32 AssistScore;

	// current weapon id, subweapon id
	int32 CurrentWeaponID;
	
	int32 CurrentSubweaponID;
};
