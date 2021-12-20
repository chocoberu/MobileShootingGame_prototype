// Fill out your copyright notice in the Description page of Project Settings.


#include "SBombSubWeapon.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ASBombSubWeapon::ASBombSubWeapon()
{
	ProjectileMovemetComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));

	ProjectileMovemetComp->InitialSpeed = 500.0f;
	ProjectileMovemetComp->MaxSpeed = 1000.0f;
	ProjectileMovemetComp->bRotationFollowsVelocity = true;
	ProjectileMovemetComp->bShouldBounce = true;
	ProjectileMovemetComp->Bounciness = 0.3f;
}

void ASBombSubWeapon::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(BombTimer,this, &ASSubWeapon::SubWeaponAttack, BombTime, false);	
}

void ASBombSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBombSubWeapon::SubWeaponAttack()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(this, BombDamage, GetActorLocation(), BombAttackRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);
	DrawDebugSphere(GetWorld(), GetActorLocation(), BombAttackRadius, 12, FColor::Yellow, false, 1.0f, 0, 1.0f);

	GetWorldTimerManager().ClearTimer(BombTimer);
	Destroy();
}
