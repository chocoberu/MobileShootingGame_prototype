// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STurret.generated.h"

UCLASS()
class MOBILESHOOTING_API ASTurret : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<class ASTurretAIController> TurretAIControllerClass;
};
