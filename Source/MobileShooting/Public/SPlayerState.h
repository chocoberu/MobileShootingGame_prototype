// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

using FOnAddKillScore = TMulticastDelegate<void(int)>;
/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ASPlayerState();

	virtual void RegisterPlayerWithSession(bool bWasFromInvite) override;

	virtual void UnregisterPlayerWithSession() override;

	uint32 GetKillScore() const { return KillScore; }

	uint32 GetDeathScore() const { return DeathScore; }

	uint32 GetAssistScore() const { return AssistScore; }

	void ResetPlayerState();

	void AddKillScore();

	void AddDeathScore();

	void AddAssistScore();

	void SetPlayerReadyState(const bool NewFlag) { bPlayerReady = NewFlag; }

	bool IsPlayerReady() const { return bPlayerReady; }

	void SetPlayerIndex(const int32 NewPlayerIndex) { PlayerIndex = NewPlayerIndex; }

	int32 GetPlayerIndex() const { return PlayerIndex; }

	int32 GetTeamNumber() const { return PlayerIndex % 2; }

	void SetWeaponId(const int32 WeaponId) { CurrentWeaponId = WeaponId; }

	void SetSubWeaponId(const int32 SubWeaponId) { CurrentSubweaponId = SubWeaponId; }

	int32 GetWeaponId() const { return CurrentWeaponId; }

	int32 GetSubWeaponId() const { return CurrentSubweaponId; }

	// Delegates
	FOnAddKillScore OnAddKillScoreDelegate;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	int32 PlayerIndex;

	bool bPlayerReady;

	UPROPERTY(Replicated)
	uint32 KillScore;

	UPROPERTY(Replicated)
	uint32 DeathScore;

	UPROPERTY(Replicated)
	uint32 AssistScore;

	// current weapon id, subweapon id
	int32 CurrentWeaponId;
	
	int32 CurrentSubweaponId;
};
