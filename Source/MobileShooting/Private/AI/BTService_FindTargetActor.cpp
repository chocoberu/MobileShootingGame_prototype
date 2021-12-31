// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTService_FindTargetActor.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"


UBTService_FindTargetActor::UBTService_FindTargetActor()
{
	AISenseSightClass = UAISense_Sight::StaticClass();
}

void UBTService_FindTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

}
