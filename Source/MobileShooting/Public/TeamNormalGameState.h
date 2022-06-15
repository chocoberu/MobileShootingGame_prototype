// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TeamNormalGameState.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ATeamNormalGameState : public AGameState
{
	GENERATED_BODY()

public:

	ATeamNormalGameState();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void SetStartGameTime();

protected:

	UPROPERTY(Replicated)
	float StartGameTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	float MaxGamePlayTime;

	float GamePlayTime;

	int32 BeforeGameTime;
	
	// UI ฐüทร
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USGameTimerHUDWidget> GameTimerWidgetClass;

	UPROPERTY()
	class USGameTimerHUDWidget* GameTimerWidget;

};
