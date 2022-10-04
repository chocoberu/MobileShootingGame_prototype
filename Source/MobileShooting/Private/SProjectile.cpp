// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASProjectile::ASProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComop"));
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));

	RootComponent = SphereComp;
	MeshComp->SetupAttachment(RootComponent);

	MeshComp->SetCollisionProfileName(TEXT("Projectile"));
	
	ProjectileMovementComp->SetUpdatedComponent(RootComponent);
	ProjectileMovementComp->InitialSpeed = 1000.0f;
	ProjectileMovementComp->MaxSpeed = 1000.0f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = true;
	ProjectileMovementComp->Bounciness = 0.3f;

	//bReplicates = true;
	//SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor == GetOwner())
	{
		return;
	}

}

void ASProjectile::SetInitialSpeed(float NewSpeed)
{
	ProjectileMovementComp->InitialSpeed = NewSpeed;
	ProjectileMovementComp->MaxSpeed = NewSpeed;
}

void ASProjectile::SetLaunchVelocity(FVector Velocity)
{
	ProjectileMovementComp->Velocity = Velocity;
}

float ASProjectile::GetProjectileRadius() const
{
	return SphereComp->GetScaledSphereRadius();
}

float ASProjectile::GetProjectileGravityZ() const
{
	return ProjectileMovementComp->GetGravityZ();
}

void ASProjectile::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	TeamId = NewTeamId;
}