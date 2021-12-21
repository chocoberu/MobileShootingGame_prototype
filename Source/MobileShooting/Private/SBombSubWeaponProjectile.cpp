// Fill out your copyright notice in the Description page of Project Settings.


#include "SBombSubWeaponProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ASBombSubWeaponProjectile::ASBombSubWeaponProjectile()
{

}

void ASBombSubWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(BombTimer, this, &ASBombSubWeaponProjectile::BombAttack, BombTime, false);

}

void ASBombSubWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBombSubWeaponProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// TODO : Overlap 할 때 폭탄을 터트릴지 말지 결정
}

void ASBombSubWeaponProjectile::BombAttack()
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(this, BombDamage, GetActorLocation(), BombAttackRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);
	DrawDebugSphere(GetWorld(), GetActorLocation(), BombAttackRadius, 12, FColor::Yellow, false, 1.0f, 0, 1.0f);

	GetWorldTimerManager().ClearTimer(BombTimer);
	Destroy();
}

