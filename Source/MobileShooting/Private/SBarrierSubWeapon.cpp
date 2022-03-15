// Fill out your copyright notice in the Description page of Project Settings.


#include "SBarrierSubWeapon.h"
#include "SBarrier.h"

ASBarrierSubWeapon::ASBarrierSubWeapon()
{

}

void ASBarrierSubWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASBarrierSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBarrierSubWeapon::StartSubWeaponAttack()
{
	if(ESubWeaponState::E_IDLE != SubWeaponState)
	{
		return;
	}

	auto BarrierObject = GetWorld()->SpawnActor<ASBarrier>(BarrierClass, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.0f, GetOwner()->GetActorRotation());

	if (nullptr != BarrierObject)
	{
		BarrierObject->SetOwner(GetOwner());
		SubtrackCurrentSubWeaponCount();
	}

	if (CurrentSubWeaponCount <= 0)
	{
		SubWeaponState = ESubWeaponState::E_RELOAD;
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASSubWeapon::ReloadSubWeapon, ReloadTime, false);
	}
	else
	{
		// TODO : SubWeapon CoolTime Àû¿ë 
	}
}

void ASBarrierSubWeapon::StopSubWeaponAttack()
{

}
