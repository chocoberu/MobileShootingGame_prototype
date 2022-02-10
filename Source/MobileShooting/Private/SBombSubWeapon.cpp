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

	// TEST CODE
	if (true == GetWorldTimerManager().IsTimerActive(BombChargingTimer))
	{
		FVector StartLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.0f;
		float RemainTime = BombMaxChargingTime - GetWorldTimerManager().GetTimerRemaining(BombChargingTimer);
		float InitialSpeed = InitialBombSpeed + ChargingBombSpeed * RemainTime / BombMaxChargingTime;
		FRotator Rot = GetOwner()->GetActorRotation();
		Rot.Pitch += 30.0f;

		FPredictProjectilePathParams PredictParams(10.0f, StartLocation, Rot.Vector() * InitialSpeed, 2.0f);
		PredictParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
		PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
		PredictParams.bTraceWithCollision = true;

		FPredictProjectilePathResult PredictResult;
		UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);

	}
}

void ASBombSubWeapon::StartSubWeaponAttack()
{
	if (true == bReload || CurrentSubWeaponCount <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("BombSubWeapon Reloading!"));
		return;
	}

	GetWorldTimerManager().SetTimer(BombChargingTimer, FTimerDelegate::CreateLambda([&]() {
		UE_LOG(LogTemp, Log, TEXT("MAX TIME !"));
		StopSubWeaponAttack();
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

	FVector StartLocation = SubWeaponOwner->GetActorLocation() + SubWeaponOwner->GetActorForwardVector() * 100.0f;

	auto Bomb = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
		StartLocation,
		Rot);

	if (nullptr != Bomb)
	{
		float InitialSpeed = InitialBombSpeed + ChargingBombSpeed * RemainTime / BombMaxChargingTime;
		UE_LOG(LogTemp, Log, TEXT("Bomb Initial Speed : %f"), InitialSpeed);
		Bomb->SetInitialSpeed(InitialSpeed);
		Bomb->SetLaunchVelocity(Rot.Vector() * InitialSpeed);

		SubtrackCurrentSubWeaponCount();

		// TEST CODE
		{
			UE_LOG(LogTemp, Log, TEXT("Bomb Vector : %s"), *(Rot.Vector() * InitialSpeed).ToString());
			FPredictProjectilePathParams PredictParams(Bomb->GetProjectileRadius(), StartLocation, Rot.Vector() * InitialSpeed, 2.0f);
			PredictParams.DrawDebugTime = 2.0f;
			PredictParams.DrawDebugType = EDrawDebugTrace::ForDuration;
			PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
			PredictParams.bTraceWithCollision = true;

			FPredictProjectilePathResult PredictResult;
			UGameplayStatics::PredictProjectilePath(Bomb, PredictParams, PredictResult);
		}
	}
	
	if (CurrentSubWeaponCount <= 0)
	{
		bReload = true;
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASSubWeapon::ReloadSubWeapon, ReloadTime, false);
	}	
}