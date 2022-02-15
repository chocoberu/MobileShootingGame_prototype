// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskNode_TestBossTeleport.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "STestBossCharacter.h"
#include "SCharacter.h"
#include "NavigationSystem.h"


UBTTaskNode_TestBossTeleport::UBTTaskNode_TestBossTeleport()
{
	NodeName = TEXT("TestBoss Teleport");
	DetectRadius = 1000.0f;
}

EBTNodeResult::Type UBTTaskNode_TestBossTeleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	// ���� �Ÿ� �̳��� ��ġ�ϸ� �ڷ���Ʈ X
	if ((TestBossCharacter->GetActorLocation() - TargetActor->GetActorLocation()).Size() <= 600.0f)
	{
		return EBTNodeResult::Succeeded;
	}

	UNavigationSystemV1* NaviSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NaviSystem)
	{
		return Result;
	}

	FNavLocation NextLocation;
	if (true == NaviSystem->GetRandomPointInNavigableRadius(TargetActor->GetActorLocation(), DetectRadius, NextLocation))
	{
		FVector Location = NextLocation.Location;
		FVector DirectionToTarget = TargetActor->GetActorLocation() - Location;
		DirectionToTarget.Z = 0.0f;
		TestBossCharacter->TeleportTo(Location, DirectionToTarget.Rotation());

		return EBTNodeResult::Succeeded;
	}

	return Result;
}
