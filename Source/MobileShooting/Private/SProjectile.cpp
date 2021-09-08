// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ASProjectile::ASProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComop"));
	ProjectileMovemetComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));

	RootComponent = SphereComp;
	MeshComp->SetupAttachment(RootComponent);

	MeshComp->SetCollisionProfileName(TEXT("Projectile"));
	
	ProjectileMovemetComp->SetUpdatedComponent(RootComponent);
	ProjectileMovemetComp->InitialSpeed = 1000.0f;
	ProjectileMovemetComp->MaxSpeed = 1000.0f;
	ProjectileMovemetComp->bRotationFollowsVelocity = true;
	ProjectileMovemetComp->bShouldBounce = true;
	ProjectileMovemetComp->Bounciness = 0.3f;

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
		return;
	
	auto Player = Cast<ACharacter>(OtherActor);
	if (Player != nullptr)
	{
		FDamageEvent DamageEvent;
		Player->TakeDamage(AttackDamage, DamageEvent, nullptr, GetOwner());
		UE_LOG(LogTemp, Log, TEXT("Projectile Overlap %s Damage : %f"), *OtherActor->GetName(), AttackDamage);
	}

	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
}

