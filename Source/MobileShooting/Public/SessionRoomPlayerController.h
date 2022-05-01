// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestSessionGameMode.h"
#include "SessionRoomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASessionRoomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	void UpdatePlayerList(const TArray<FRoomPlayerInfo>& PlayerInfoList);

	void RequestServerPlayerList();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestServerPlayerList();
	
	void ChangeReadyState();

	void ReadyGame(bool bReadyState);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReadyGame(bool bReadyState);

	UFUNCTION(Client, Reliable)
	void Client_ReadyGame(bool bReadyState);

	void StartGame();

	UFUNCTION(Client, Reliable)
	void Client_UpdatePlayerList(const TArray<FRoomPlayerInfo>& PlayerInfoList);

	void SetPlayerName(const FString NewName);

	FString GetPlayerName();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UTestSessionRoomWidget> SessionRoomUIClass;

	UPROPERTY()
	class UTestSessionRoomWidget* SessionRoomWidget;
};
