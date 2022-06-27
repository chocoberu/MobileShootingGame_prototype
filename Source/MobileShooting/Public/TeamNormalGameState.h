// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TeamNormalGameState.generated.h"

using FOnAllPlayerReadyDelegate = TMulticastDelegate<void()>;
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

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_CountDown(int32 CountDownNumber);

	bool IsAllPlayerReadyState();
	
	// Delegates
	FOnAllPlayerReadyDelegate OnAllPlayerReadyDelegate;

protected:

	// Game Time 관련
	UPROPERTY(Replicated)
	float StartGameTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	float MaxGamePlayTime;

	float CurrentGamePlayTime;

	int32 BeforeGameTime;

	// 팀
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	int32 TargetKillCount;

	UPROPERTY(ReplicatedUsing = OnRep_BlueTeamKillCount)
	int32 BlueTeamKillCount;

	UPROPERTY(ReplicatedUsing = OnRep_RedTeamKillCount)
	int32 RedTeamKillCount;
	
	// UI 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USGameTimerHUDWidget> GameTimerWidgetClass;

	UPROPERTY()
	class USGameTimerHUDWidget* GameTimerWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UTeamScoreWidget> TeamScoreWidgetClass;

	UPROPERTY()
	class UTeamScoreWidget* TeamScoreWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UMatchStartCountDownWidget> MatchStartCountDownWidgetClass;

	UPROPERTY()
	class UMatchStartCountDownWidget* MatchStartCountDownWidget;

	// Match CountDown 관련
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	int32 MaxCountDown;

	int32 CurrentCountDown;

	bool bAllPlayerReady;
};
