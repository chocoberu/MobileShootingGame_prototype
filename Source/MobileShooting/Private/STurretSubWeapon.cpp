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
	// TODO : Turret�� Owner ��ġ�� Spawn

	auto TurretObject = GetWorld()->SpawnActor<ASTurret>(TurretClass, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.0f, GetOwner()->GetActorRotation());

	if (TurretObject != nullptr)
	{
		TurretObject->SetOwner(GetOwner());
	}
}

void ASTurretSubWeapon::StopSubWeaponAttack()
{
	// Stop������ ���� X
}
