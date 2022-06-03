// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "SGameSession.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASGameSession : public AGameSession
{
	GENERATED_BODY()
	
	virtual void UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId) override;
};
