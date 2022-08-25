// Fill out your copyright notice in the Description page of Project Settings.


#include "SBombSubWeaponProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASBombSubWeaponProjectile::ASBombSubWeaponProjectile()
{

}

void ASBombSubWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	//GetWorldTimerManager().SetTimer(BombTimer, this, &ASBombSubWeaponProjectile::BombAttack, BombTime, false);

}

void ASBombSubWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("Curreunt Velocity : %s, len : %f"), *ProjectileMovemetComp->Velocity.ToString(), ProjectileMovemetComp->Velocity.Size());
}

void ASBombSubWeaponProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	BombAttack(OtherActor);
}

void ASBombSubWeaponProjectile::BombAttack(AActor* OtherActor)
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	//UGameplayStatics::ApplyRadialDamage(this, BombDamage, GetActorLocation(), BombAttackRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);
	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(BombDamage, DamageEvent, nullptr, GetOwner());
	DrawDebugSphere(GetWorld(), GetActorLocation(), BombAttackRadius, 12, FColor::Yellow, false, 1.0f, 0, 1.0f);

	GetWorldTimerManager().ClearTimer(BombTimer);
	Destroy();
}

