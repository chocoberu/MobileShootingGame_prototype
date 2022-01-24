// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_TestBossNormalAttack.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_TestBossNormalAttack::UBTTaskNode_TestBossNormalAttack()
{
	NodeName = TEXT("TestBoss NormalAttack");

}

EBTNodeResult::Type UBTTaskNode_TestBossNormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
