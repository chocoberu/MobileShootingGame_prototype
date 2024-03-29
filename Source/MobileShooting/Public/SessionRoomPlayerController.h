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

	void LeaveSession();

	UFUNCTION(Client, Reliable)
	void Client_LeaveSession(bool bKicked = false);

	void SetPlayerName(const FString& NewName);

	UFUNCTION(Client, Reliable)
	void Client_SetPlayerName(const FString& NewName);

	FString GetPlayerName();

	bool IsPlayerReady() const;

	bool IsSessionHost() const;

	void SetWeaponId(const int32 WeaponId);

	void SetSubWeaponId(const int32 SubWeaponId);

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UTestSessionRoomWidget> SessionRoomUIClass;

	UPROPERTY()
	class UTestSessionRoomWidget* SessionRoomWidget;

	bool bIsSessionHost;
};
