// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_TurretAttack.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UBTTaskNode_TurretAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTaskNode_TurretAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
