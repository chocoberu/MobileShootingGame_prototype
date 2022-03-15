// Fill out your copyright notice in the Description page of Project Settings.


#include "STurretSubWeapon.h"
#include "STurret.h"
#include "SCharacter.h"
#include "AI/STurretAIController.h"

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
	if (ESubWeaponState::E_IDLE != SubWeaponState || CurrentSubWeaponCount <= 0)
	{
		return;
	}

	auto TurretObject = GetWorld()->SpawnActor<ASTurret>(TurretClass, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.0f, GetOwner()->GetActorRotation());
	auto SOwner = Cast<ASCharacter>(GetOwner());

	if (nullptr != TurretObject && nullptr != SOwner)
	{
		TurretObject->SetOwner(GetOwner());
		SubtrackCurrentSubWeaponCount();
	}

	if (CurrentSubWeaponCount <= 0)
	{
		SubWeaponState = ESubWeaponState::E_RELOAD;
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASSubWeapon::ReloadSubWeapon, ReloadTime, false);
	}
}

void ASTurretSubWeapon::StopSubWeaponAttack()
{
	// Stop에서는 수행 X
}
