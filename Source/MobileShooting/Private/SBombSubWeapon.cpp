// Fill out your copyright notice in the Description page of Project Settings.


#include "SBombSubWeapon.h"
#include "SBombSubWeaponProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ASBombSubWeapon::ASBombSubWeapon()
{
	
}

void ASBombSubWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASBombSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBombSubWeapon::StartSubWeaponAttack()
{
	if (true == bReload)
	{
		UE_LOG(LogTemp, Log, TEXT("BombSubWeapon Reloading!"));
		return;
	}

	GetWorldTimerManager().SetTimer(BombChargingTimer, FTimerDelegate::CreateLambda([&]() {
		UE_LOG(LogTemp, Log, TEXT("MAX TIME !"));
		}), BombMaxChargingTime, false);
}

void ASBombSubWeapon::StopSubWeaponAttack()
{
	// Stop老 锭 气藕阑 带瘤绰 规侥

	if (false == GetWorldTimerManager().IsTimerActive(BombChargingTimer))
	{
		return;
	}

	GetWorldTimerManager().PauseTimer(BombChargingTimer);
	float RemainTime = BombMaxChargingTime - GetWorldTimerManager().GetTimerRemaining(BombChargingTimer);

	UE_LOG(LogTemp, Log, TEXT("%f Time Left"), RemainTime);

	GetWorldTimerManager().ClearTimer(BombChargingTimer);

	auto SubWeaponOwner = GetOwner();
	FRotator Rot = SubWeaponOwner->GetActorRotation();
	Rot.Pitch += 30.0f;

	auto Bomb = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
		SubWeaponOwner->GetActorLocation() + SubWeaponOwner->GetActorForwardVector() * 100.0f,
		Rot);

	if (nullptr != Bomb)
	{
		Bomb->SetInitialSpeed(1000.0f + 2000.0f * RemainTime / BombMaxChargingTime);
		SubtrackCurrentSubWeaponCount();
	}
	

	if (0 >= CurrentSubWeaponCount)
	{
		bReload = true;
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASSubWeapon::ReloadSubWeapon, ReloadTime, false);
	}	
}