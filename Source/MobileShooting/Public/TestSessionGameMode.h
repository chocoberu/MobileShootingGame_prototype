// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestSessionGameMode.generated.h"

USTRUCT(BlueprintType)
struct FRoomPlayerInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	bool bPlayerReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerInfo")
	int32 TeamNumber;
};

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ATestSessionGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ATestSessionGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	void UpdatePlayerList();

	void StartGame();

	void LeaveSession(const FString PlayerName);

protected:

	TArray<class ASessionRoomPlayerController*> PlayerControllerList;

	TArray<FRoomPlayerInfo> PlayerInfoList;

	uint32 PlayerCount;
};
