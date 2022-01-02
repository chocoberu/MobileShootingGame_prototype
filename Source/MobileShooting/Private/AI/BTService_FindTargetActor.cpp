// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTService_FindTargetActor.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "SCharacter.h"


UBTService_FindTargetActor::UBTService_FindTargetActor()
{
	AISenseSightClass = UAISense_Sight::StaticClass();

	NodeName = TEXT("Find Target Actor");
}

void UBTService_FindTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ControllingPawn == nullptr)
		return;

	auto AIPerceptionComp = ControllingPawn->FindComponentByClass<UAIPerceptionComponent>();
	if (AIPerceptionComp == nullptr)
		return;

	UE_LOG(LogTemp, Log, TEXT("AIPerception Comp Owner : %s"), *AIPerceptionComp->GetOwner()->GetName());

	TArray<AActor*> ActorArray;
	AIPerceptionComp->GetKnownPerceivedActors(nullptr, ActorArray);

	//UE_LOG(LogTemp, Log, TEXT("Perceived Actors : %d"), ActorArray.Num());
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);

	float Length = 100000.0f;

	AActor* FinalTarget = nullptr;

	for (auto TargetActor : ActorArray)
	{
		if (TargetActor != nullptr)
		{
			auto PlayerCharacter = Cast<ASCharacter>(TargetActor);
			if (PlayerCharacter == nullptr)
				continue;

			if (Length > (ControllingPawn->GetActorLocation() - TargetActor->GetActorLocation()).Size())
			{
				Length = (ControllingPawn->GetActorLocation() - TargetActor->GetActorLocation()).Size();
				FinalTarget = TargetActor;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), FinalTarget);
}
