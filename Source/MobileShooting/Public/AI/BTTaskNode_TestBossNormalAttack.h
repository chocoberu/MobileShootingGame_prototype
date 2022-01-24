// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_TestBossNormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UBTTaskNode_TestBossNormalAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_TestBossNormalAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


};
