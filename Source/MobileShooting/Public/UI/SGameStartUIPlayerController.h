// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SGameStartUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASGameStartUIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASGameStartUIPlayerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> GameStartUIClass;

	UPROPERTY()
	class UUserWidget* GameStartUI;
};
