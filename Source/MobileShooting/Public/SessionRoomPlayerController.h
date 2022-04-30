// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SessionRoomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASessionRoomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	void UpdatePlayerList();

	void RequestServerPlayerList();

	void ReadyGame();

	void StartGame();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UTestSessionRoomWidget> SessionRoomUIClass;

	UPROPERTY()
	class UTestSessionRoomWidget* SessionRoomWidget;
};
