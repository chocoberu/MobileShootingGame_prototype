// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "STurretAIController.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASTurretAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ASTurretAIController();

	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void BeginPlay() override;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	void RunAI();
	void StopAI();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBehaviorTree* BTAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UBlackboardData* BBAsset;

	FGenericTeamId TeamId;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
};
