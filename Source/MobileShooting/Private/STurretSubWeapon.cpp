// Fill out your copyright notice in the Description page of Project Settings.


#include "STurretSubWeapon.h"
#include "STurret.h"

ASTurretSubWeapon::ASTurretSubWeapon()
{

}

void ASTurretSubWeapon::BeginPlay()
{
	Super::BeginPlay();

	ReloadTurret();
}

void ASTurretSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTurretSubWeapon::StartSubWeaponAttack()
{
	if (bIsReload == true)
	{
		return;
	}

	// TODO : Turret을 Owner 위치에 Spawn

	auto TurretObject = GetWorld()->SpawnActor<ASTurret>(TurretClass, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.0f, GetOwner()->GetActorRotation());

	if (TurretObject != nullptr)
	{
		TurretObject->SetOwner(GetOwner());

		CurrentTurretCount--;
	}

	if (CurrentTurretCount <= 0)
	{
		bIsReload = true;
		GetWorldTimerManager().SetTimer(TurretReloadTimer, this, &ASTurretSubWeapon::ReloadTurret, TurretReloadTime, false);
	}
}

void ASTurretSubWeapon::StopSubWeaponAttack()
{
	// Stop에서는 수행 X
}

void ASTurretSubWeapon::ReloadTurret()
{
	bIsReload = false;
	CurrentTurretCount = DefaultTurretCount;
	GetWorldTimerManager().ClearTimer(TurretReloadTimer);

	UE_LOG(LogTemp, Log, TEXT("TurretSubWeapon Reload Complete"));
}
