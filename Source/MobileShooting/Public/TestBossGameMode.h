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

	void StartTimer();

	UFUNCTION()
	void UpdateTimeCount();

	int32 GetCurrentRemainTime() { return CurrentRemainTime; }

protected:
	UPROPERTY()
	class ATestBossGameState* TestGameState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USGameTimerHUDWidget> GameTimerWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USGameQuestTextWidget> GameQuestTextWidgetClass;

	UPROPERTY()
	class USGameTimerHUDWidget* GameTimerWidget;

	UPROPERTY()
	class USGameQuestTextWidget* GameQuestTextWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	int32 DefaultRemainTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FString GameQuestString;

	int32 CurrentRemainTime;

	FTimerHandle GameTimer;
};
