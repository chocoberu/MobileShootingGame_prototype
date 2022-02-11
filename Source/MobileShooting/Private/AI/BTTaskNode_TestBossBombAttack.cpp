// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_TestBossBombAttack.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "STestBossCharacter.h"
#include "SCharacter.h"

UBTTaskNode_TestBossBombAttack::UBTTaskNode_TestBossBombAttack()
{
	NodeName = TEXT("TestBoss BombAttack");
	bNotifyTick = true;
	bAttack = false;
}

EBTNodeResult::Type UBTTaskNode_TestBossBombAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	RemainAttackTime = BombAttackCoolTime;
	bAttack = true;

	UE_LOG(LogTemp, Log, TEXT("Boss bomb attack start"));
	return EBTNodeResult::InProgress;
}

void UBTTaskNode_TestBossBombAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (false == bAttack)
	{
		return;
	}

	RemainAttackTime -= DeltaSeconds;

	if (RemainAttackTime <= 0.0f)
	{
		auto TestBossCharacter = Cast<ASTestBossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
		if (nullptr == TestBossCharacter)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			return;
		}
		TestBossCharacter->BombAttack();
		
		bAttack = false;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		UE_LOG(LogTemp, Log, TEXT("Boss bomb attack end"));
	}
}
