// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STestBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ASTestBossAIController::ASTestBossAIController()
{

}

void ASTestBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ASTestBossAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASTestBossAIController::RunAI()
{
	UseBlackboard(BBAsset, Blackboard);
	RunBehaviorTree(BTAsset);

	UE_LOG(LogTemp, Log, TEXT("TestBossAiController Run"));
}

void ASTestBossAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}
