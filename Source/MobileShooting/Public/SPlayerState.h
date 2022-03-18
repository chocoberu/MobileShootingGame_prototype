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

protected:

	uint32 KillScore;

	uint32 DeathScore;

	uint32 AssistScore;
};
