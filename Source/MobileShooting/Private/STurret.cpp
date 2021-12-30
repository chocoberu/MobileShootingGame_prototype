// Fill out your copyright notice in the Description page of Project Settings.


#include "STurret.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AI/STurretAIController.h"

// Sets default values
ASTurret::ASTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComop"));
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	RootComponent = CapsuleComp;
	MeshComp->SetupAttachment(RootComponent);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ASTurret::BeginPlay()
{
	Super::BeginPlay();
	
	auto AIController = Cast<ASTurretAIController>(GetController());

	if (AIController != nullptr)
	{
		AIController->RunAI();

		//UE_LOG(LogTemp, Log, TEXT("Turret AI Run"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller is nullptr"));
	}
	
}

// Called every frame
void ASTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}