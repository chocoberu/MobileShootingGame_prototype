// Fill out your copyright notice in the Description page of Project Settings.


#include "SBombSubWeapon.h"
#include "SBombSubWeaponProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ASBombSubWeapon::ASBombSubWeapon()
{
	
}

void ASBombSubWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASBombSubWeapon::PredictBombPath(ASProjectile* Bomb, FVector StartLocation, FRotator Rotation, float InitialSpeed, float PredictTime)
{
	FPredictProjectilePathParams PredictParams;
	if (nullptr == Bomb)
	{
		PredictParams = FPredictProjectilePathParams(10.0f, StartLocation, Rotation.Vector() * InitialSpeed, PredictTime);
		PredictParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	}
	else
	{
		PredictParams = FPredictProjectilePathParams(Bomb->GetProjectileRadius(), StartLocation, Rotation.Vector() * InitialSpeed, PredictTime);
		PredictParams.DrawDebugType = EDrawDebugTrace::ForDuration;
	}
	PredictParams.DrawDebugTime = PredictTime;
	PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
	PredictParams.bTraceWithCollision = true;
	FPredictProjectilePathResult PredictResult;

	if (nullptr == Bomb)
	{
		UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);
	}
	else
	{
		UGameplayStatics::PredictProjectilePath(Bomb, PredictParams, PredictResult);
	}
}

void ASBombSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Charging이면 Bomb 예측 경로를 출력
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (nullptr == PawnOwner)
	{
		return;
	}

	if (true == GetWorldTimerManager().IsTimerActive(BombChargingTimer) && true == PawnOwner->IsLocallyControlled())
	{
		FVector StartLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.0f;
		float RemainTime = BombMaxChargingTime - GetWorldTimerManager().GetTimerRemaining(BombChargingTimer);
		float InitialSpeed = InitialBombSpeed + ChargingBombSpeed * RemainTime / BombMaxChargingTime;
		FRotator Rot = GetOwner()->GetActorRotation();
		Rot.Pitch += 30.0f;

		PredictBombPath(nullptr, StartLocation, Rot, InitialSpeed, 2.0f);
	}
}

void ASBombSubWeapon::StartSubWeaponAttack()
{
	if (ESubWeaponState::E_IDLE != SubWeaponState || CurrentSubWeaponCount <= 0)
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
	// Stop일 때 폭탄을 던지는 방식
	if (false == GetWorldTimerManager().IsTimerActive(BombChargingTimer))
	{
		return;
	}

	GetWorldTimerManager().PauseTimer(BombChargingTimer);
	float RemainTime = BombMaxChargingTime - GetWorldTimerManager().GetTimerRemaining(BombChargingTimer);

	UE_LOG(LogTemp, Log, TEXT("%f Time Left"), RemainTime);

	GetWorldTimerManager().ClearTimer(BombChargingTimer);

	ASCharacter* SubWeaponOwner = Cast<ASCharacter>(GetOwner());
	FRotator Rot = SubWeaponOwner->GetActorRotation();
	Rot.Pitch += 30.0f;

	FVector StartLocation = SubWeaponOwner->GetActorLocation() + SubWeaponOwner->GetActorForwardVector() * 100.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = SubWeaponOwner;
	auto Bomb = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
		StartLocation,
		Rot,
		SpawnParams);

	if (nullptr != Bomb)
	{
		Bomb->SetGenericTeamId(SubWeaponOwner->GetGenericTeamId());

		float InitialSpeed = InitialBombSpeed + ChargingBombSpeed * RemainTime / BombMaxChargingTime;
		UE_LOG(LogTemp, Log, TEXT("Bomb Initial Speed : %f"), InitialSpeed);
		Bomb->SetInitialSpeed(InitialSpeed);
		Bomb->SetLaunchVelocity(Rot.Vector() * InitialSpeed);

		Multicast_OnSubWeaponAttack();
	}
}

void ASBombSubWeapon::OnSubWeaponAttack()
{
	Super::OnSubWeaponAttack();

	/* {
		UE_LOG(LogTemp, Log, TEXT("Bomb Vector : %s"), *(Rot.Vector() * InitialSpeed).ToString());
		PredictBombPath(Bomb, StartLocation, Rot, InitialSpeed, 2.0f);
	}*/

	if (CurrentSubWeaponCount <= 0)
	{
		SubWeaponState = ESubWeaponState::E_RELOAD;
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASSubWeapon::ReloadSubWeapon, ReloadTime, false);
	}
}

void ASBombSubWeapon::Client_StartSubWeaponAttack()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (false == PawnOwner->IsLocallyControlled())
	{
		return;
	}

	GetWorldTimerManager().SetTimer(BombChargingTimer, FTimerDelegate::CreateLambda([&]() {
		Client_StopSubWeaponAttack();
		}), BombMaxChargingTime, false);
}

void ASBombSubWeapon::Client_StopSubWeaponAttack()
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (false == PawnOwner->IsLocallyControlled())
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(BombChargingTimer);
}