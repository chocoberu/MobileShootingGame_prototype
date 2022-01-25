// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_TestBossNormalAttack.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "STestBossCharacter.h"
#include "SCharacter.h"

UBTTaskNode_TestBossNormalAttack::UBTTaskNode_TestBossNormalAttack()
{
	NodeName = TEXT("TestBoss NormalAttack");

}

EBTNodeResult::Type UBTTaskNode_TestBossNormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}
	
	auto TestBossCharacter = Cast<ASTestBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	auto TargetActor = Cast<ASCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (nullptr == TargetActor || nullptr == TestBossCharacter)
	{
		return EBTNodeResult::Failed;
	}

	TestBossCharacter->NormalAttack();

	return Result;
}
