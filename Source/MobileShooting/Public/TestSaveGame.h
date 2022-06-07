// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TestSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTestSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UTestSaveGame();

public:

	UPROPERTY()
	int32 MainWeaponId;

	UPROPERTY()
	int32 SubWeaponId;

	UPROPERTY()
	int32 PlayerIndex;
};
