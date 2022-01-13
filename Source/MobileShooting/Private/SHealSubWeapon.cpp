// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealSubWeapon.h"
#include "SCharacter.h"
#include "Components/SHealthComponent.h"

ASHealSubWeapon::ASHealSubWeapon()
{

}

void ASHealSubWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASHealSubWeapon::Heal()
{
	SubtrackCurrentSubWeaponCount();

	if (0 > CurrentSubWeaponCount)
	{
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASSubWeapon::ReloadSubWeapon, ReloadTime, false);
		return;
	}

	auto SubWeaponOwner = Cast<ASCharacter>(GetOwner());
	if (nullptr == SubWeaponOwner)
	{
		UE_LOG(LogTemp, Error, TEXT("SubWeaponOwner is nullptr"));
		return;
	}
	auto HealthComp = SubWeaponOwner->GetHealthComponent();
	if (nullptr == HealthComp)
	{
		UE_LOG(LogTemp, Error, TEXT("HealthComp is nullptr"));
		return;
	}

	HealthComp->SetHealth(HealthComp->GetHealth() + HealAmount);
	UE_LOG(LogTemp, Log, TEXT("HealSubWeapon Count : %d, Current HP : %f"), CurrentSubWeaponCount, HealthComp->GetHealth());
	GetWorldTimerManager().SetTimer(HealTickTimer, this, &ASHealSubWeapon::Heal, HealTick, false);
}

void ASHealSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASHealSubWeapon::StartSubWeaponAttack()
{
	if (true == bReload)
	{
		return;
	}

	Heal();
	bReload = true;
}

void ASHealSubWeapon::StopSubWeaponAttack()
{

}
