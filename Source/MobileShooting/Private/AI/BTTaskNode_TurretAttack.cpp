// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_TurretAttack.h"

UBTTaskNode_TurretAttack::UBTTaskNode_TurretAttack()
{
	NodeName = TEXT("Turret Attack");
}

EBTNodeResult::Type UBTTaskNode_TurretAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTaskNode_TurretAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
