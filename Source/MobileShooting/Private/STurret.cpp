// Fill out your copyright notice in the Description page of Project Settings.


#include "STurret.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SHealthComponent.h"
#include "AI/STurretAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "SProjectile.h"
#include "SCharacter.h"

// Sets default values
ASTurret::ASTurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	WeaponMuzzleSocketName = TEXT("Muzzle");

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	AIPerceptionComp->ConfigureSense(*SightConfig);
	AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddUObject(this, &ASTurret::OnHealthChanged);
	
	RootComponent = CapsuleComp;
	MeshComp->SetupAttachment(RootComponent);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// TEST CODE
	TeamId = FGenericTeamId(0);
}

// Called when the game starts or when spawned
void ASTurret::BeginPlay()
{
	Super::BeginPlay();
	
	auto AIController = Cast<ASTurretAIController>(GetController());

	if (nullptr != AIController)
	{
		AIController->RunAI();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AI Controller is nullptr"));
	}

	SetLifeSpan(TurretLifeSpan);
}

// Called every frame
void ASTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTurret::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (NewTeamID != FGenericTeamId::NoTeam)
	{
		TeamId = NewTeamID;
	}
}

ETeamAttitude::Type ASTurret::GetTeamAttitudeTowards(const AActor& Other) const
{
	UE_LOG(LogTemp, Log, TEXT("GetTeamAttitudeTowards() Call : Other : %s"), *Other.GetName());

	// TODO : Controller 관련 작업 추가?, Neutral 관련 작업 추가 (현재 Actor에 FGenericTeamId를 통해 체크)

	ETeamAttitude::Type Result = IGenericTeamAgentInterface::GetTeamAttitudeTowards(Other);
	return Result;
}

void ASTurret::TurretAttack()
{
	ASProjectile* Bullet = GetWorld()->SpawnActor<ASProjectile>(TurretProjectileClass,
		MeshComp->GetSocketLocation(WeaponMuzzleSocketName),
		MeshComp->GetSocketRotation(WeaponMuzzleSocketName));
	
}

void ASTurret::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;

		auto AIController = Cast<ASTurretAIController>(GetController());
		if (nullptr != AIController)
		{
			AIController->StopAI();
		}

		Destroy();
	}
}
