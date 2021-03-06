// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestBossGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ATestBossGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATestBossGameMode();

	virtual void PostInitializeComponents() override;
	
	virtual void StartPlay() override;
	
	virtual void BeginPlay() override;

	virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

	virtual bool ClearPause() override;

	void SetGameClear(bool Value);

protected:
	UPROPERTY()
	class ATestBossGameState* TestGameState;

};
