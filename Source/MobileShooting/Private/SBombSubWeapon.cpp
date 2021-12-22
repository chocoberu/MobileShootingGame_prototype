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

	CurrentBombCount = DefaultBombCount;
	bIsReload = false;
}

void ASBombSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBombSubWeapon::StartSubWeaponAttack()
{
	if (bIsReload == true)
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
	// Stop�� �� ��ź�� ������ ���

	if (GetWorldTimerManager().IsTimerActive(BombChargingTimer) == false)
	{
		return;
	}

	GetWorldTimerManager().PauseTimer(BombChargingTimer);
	float RemainTime = BombMaxChargingTime - GetWorldTimerManager().GetTimerRemaining(BombChargingTimer);

	UE_LOG(LogTemp, Log, TEXT("%f Time Left"), RemainTime);

	GetWorldTimerManager().ClearTimer(BombChargingTimer);

	auto SubWeaponOwner = GetOwner();

	auto Bomb = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
		SubWeaponOwner->GetActorLocation() + SubWeaponOwner->GetActorForwardVector() * 100.0f,
		SubWeaponOwner->GetActorRotation());

	if (Bomb != nullptr)
	{
		CurrentBombCount--;
	}

	if (CurrentBombCount == 0)
	{
		bIsReload = true;
		GetWorldTimerManager().SetTimer(BombReloadTimer, this, &ASBombSubWeapon::ReloadBomb, BombReloadTime, false);
	}

	
}

void ASBombSubWeapon::ReloadBomb()
{
	CurrentBombCount = DefaultBombCount;
	bIsReload = false;
	GetWorldTimerManager().ClearTimer(BombReloadTimer);

	UE_LOG(LogTemp, Log, TEXT("BombSubWeapon Reload Complete"));
}
