// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TeamNormalGameState.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ATeamNormalGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	ATeamNormalGameState();

protected:

	UPROPERTY(Replicated)
	int32 CurrentGameTime;
	
	
};
