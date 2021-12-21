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
}

void ASBombSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBombSubWeapon::StartSubWeaponAttack()
{
	// TODO : Start ���� Stop���� �ð��� ���

	// �׽�Ʈ �ڵ�
	GetWorldTimerManager().SetTimer(BombChargingTimer, FTimerDelegate::CreateLambda([&]() {
		UE_LOG(LogTemp, Log, TEXT("MAX TIME !"));
		}), BombMaxChargingTime, false);
}

void ASBombSubWeapon::StopSubWeaponAttack()
{
	// Stop�� �� ��ź�� ������ ���

	// �׽�Ʈ �ڵ�
	GetWorldTimerManager().PauseTimer(BombChargingTimer);
	float RemainTime = GetWorldTimerManager().GetTimerRemaining(BombChargingTimer);

	UE_LOG(LogTemp, Log, TEXT("%f Time Left"), RemainTime);

	GetWorldTimerManager().ClearTimer(BombChargingTimer);

	if (CurrentBombCount == 0)
	{
		GetWorldTimerManager().SetTimer(BombReloadTimer, this, &ASBombSubWeapon::ReloadBomb, BombReloadTime, false);
	}

	
}

void ASBombSubWeapon::ReloadBomb()
{
	CurrentBombCount = DefaultBombCount;

	GetWorldTimerManager().ClearTimer(BombReloadTimer);
}
