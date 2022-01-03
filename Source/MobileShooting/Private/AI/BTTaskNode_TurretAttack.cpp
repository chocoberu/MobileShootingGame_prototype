// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_TurretAttack.h"
#include "STurret.h"
#include "SCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_TurretAttack::UBTTaskNode_TurretAttack()
{
	NodeName = TEXT("Turret Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_TurretAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	CurrentAttackCount = DefaultAttackCount;

	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}

	TurretActor = Cast<ASTurret>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == TurretActor)
	{
		return EBTNodeResult::Failed;
	}

	auto TargetActor = Cast<ASCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (nullptr == TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	// TODO : Turret 회전 구현

	TurretAttack();
	
	return EBTNodeResult::InProgress;
}

void UBTTaskNode_TurretAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (CurrentAttackCount <= 0)
	{
		CurrentAttackCount = DefaultAttackCount;
		GetWorld()->GetTimerManager().ClearTimer(TurretAttackTimer);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UBTTaskNode_TurretAttack::TurretAttack()
{
	if (false == IsValid(TurretActor))
	{
		return;
	}

	TurretActor->TurretAttack();
	--CurrentAttackCount;

	if (CurrentAttackCount > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(TurretAttackTimer, this, &UBTTaskNode_TurretAttack::TurretAttack, AttackCoolTime, false);
	}
}
