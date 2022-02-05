// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STestBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASTestBossAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ASTestBossAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	void RunAI();
	void StopAI();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBlackboardData* BBAsset;
};
