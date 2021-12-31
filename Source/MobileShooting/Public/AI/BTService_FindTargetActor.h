// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UBTService_FindTargetActor : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_FindTargetActor();

	UPROPERTY(EditAnywhere, Category = "Class")
	TSubclassOf<class UAISense> AISenseSightClass;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
