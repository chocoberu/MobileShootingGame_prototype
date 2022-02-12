// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STurretAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "SCharacter.h"

ASTurretAIController::ASTurretAIController()
{

}

void ASTurretAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Log, TEXT("TurretAIController OnPossess"));
}

void ASTurretAIController::BeginPlay()
{
	Super::BeginPlay();

	auto PossessCharacter = Cast<ASCharacter>(GetPawn());
	if (nullptr != PossessCharacter)
	{
		TeamId = FGenericTeamId(0);
	}
}

void ASTurretAIController::RunAI()
{
	UseBlackboard(BBAsset, Blackboard);
	RunBehaviorTree(BTAsset);

	UE_LOG(LogTemp, Log, TEXT("TurretAiController Run"));
}

void ASTurretAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

ETeamAttitude::Type ASTurretAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	if (nullptr == OtherPawn)
	{
		return ETeamAttitude::Neutral;
	}

	auto PlayerTeamInterface = Cast<IGenericTeamAgentInterface>(&Other);
	if (nullptr == PlayerTeamInterface)
	{
		return ETeamAttitude::Neutral;
	}
	FGenericTeamId OtherTeamId = PlayerTeamInterface->GetGenericTeamId();

	if (OtherTeamId == GetGenericTeamId())
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Neutral;
}
