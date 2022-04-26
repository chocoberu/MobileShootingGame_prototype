// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestSessionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ATestSessionGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ATestSessionGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
