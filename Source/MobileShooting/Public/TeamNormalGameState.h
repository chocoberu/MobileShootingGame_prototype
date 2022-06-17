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

	float GetCurrentGamePlayTime() const;

	UFUNCTION()
	void OnRep_BlueTeamKillCount();

	UFUNCTION()
	void OnRep_RedTeamKillCount();

protected:

	// Game Time ฐüทร
	UPROPERTY(Replicated)
	float StartGameTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	float MaxGamePlayTime;

	float CurrentGamePlayTime;

	int32 BeforeGameTime;

	// ฦภ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	int32 TargetKillCount;

	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamKillCount)
	int32 BlueTeamKillCount;

	UPROPERTY(ReplicatedUsing = OnRep_RedTeamKillCount)
	int32 RedTeamKillCount;
	
	// UI ฐüทร
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USGameTimerHUDWidget> GameTimerWidgetClass;

	UPROPERTY()
	class USGameTimerHUDWidget* GameTimerWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UTeamScoreWidget> TeamScoreWidgetClass;

	UPROPERTY()
	class UTeamScoreWidget* TeamScoreWidget;

};
