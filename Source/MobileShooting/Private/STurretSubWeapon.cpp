// Fill out your copyright notice in the Description page of Project Settings.


#include "STurretSubWeapon.h"
#include "STurret.h"

ASTurretSubWeapon::ASTurretSubWeapon()
{

}

void ASTurretSubWeapon::BeginPlay()
{
	Super::BeginPlay();

}

void ASTurretSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTurretSubWeapon::StartSubWeaponAttack()
{
	if (true == bReload || CurrentSubWeaponCount <= 0)
	{
		return;
	}

	auto TurretObject = GetWorld()->SpawnActor<ASTurret>(TurretClass, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.0f, GetOwner()->GetActorRotation());

	if (nullptr != TurretObject)
	{
		TurretObject->SetOwner(GetOwner());

		SubtrackCurrentSubWeaponCount();
	}

	if (CurrentSubWeaponCount <= 0)
	{
		bReload = true;
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASSubWeapon::ReloadSubWeapon, ReloadTime, false);
	}
}

void ASTurretSubWeapon::StopSubWeaponAttack()
{
	// Stop에서는 수행 X
}
