// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestSessionGameMode.generated.h"

// Session�� ������ Player�� ���õ� ������ �����ϴ� ����ü
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

	// Session�� ������ �÷��̾� ��Ʈ�ѷ��� �����ϴ� TArray
	TArray<class ASessionRoomPlayerController*> PlayerControllerList;

	// UpdatePlayerList()���� ����ϴ� TArray, Player�� ���õ� ������ ����
	TArray<FRoomPlayerInfo> PlayerInfoList;

	// ���� �� �ο� ���� �����ϴ� ������
	int32 BlueTeamCount;

	int32 RedTeamCount;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	int32 MaxPlayerCount;
};
