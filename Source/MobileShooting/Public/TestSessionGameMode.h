// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestSessionGameMode.generated.h"

// Session에 참여한 Player와 관련된 정보를 보관하는 구조체
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
	int32 PlayerIndex;
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

	bool StartGame();

	void LeaveSession();

protected:

	// Session에 참여한 플레이어 컨트롤러를 보관하는 TArray
	TArray<class ASessionRoomPlayerController*> PlayerControllerList;

	// UpdatePlayerList()에서 사용하는 TArray, Player와 관련된 정보를 보관
	TArray<FRoomPlayerInfo> PlayerInfoList;

	// 현재 팀 인원 수를 저장하는 변수들
	int32 BlueTeamCount;

	int32 RedTeamCount;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int32 MaxPlayerCount;
};
