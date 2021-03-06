// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TeamNormalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ATeamNormalGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ATeamNormalGameMode();

	virtual void InitGameState() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void RestartPlayer(AController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void StartMatch() override;

	virtual void Tick(float DeltaSeconds) override;

	void CountForStartMatch();

	virtual void EndMatch() override;

	void ResponseRestartPlayer(AController* NewPlayer);

private:

	TArray<class ASPlayerController*> PlayerControllerList;

	int32 StartCount;

	FTimerHandle StartTimer;
};
