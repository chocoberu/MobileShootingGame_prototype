// Fill out your copyright notice in the Description page of Project Settings.


#include "STurretSubWeapon.h"

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
	// TODO : Turret을 Owner 위치에 Spawn
}

void ASTurretSubWeapon::StopSubWeaponAttack()
{
	// Stop에서는 수행 X
}
