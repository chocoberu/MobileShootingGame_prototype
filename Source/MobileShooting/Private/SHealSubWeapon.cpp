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

	ReloadHealSubWeapon();
}

void ASHealSubWeapon::Heal()
{
	--CurrentHealCount;

	if (0 > CurrentHealCount)
	{
		GetWorldTimerManager().SetTimer(HealCoolTimer, this, &ASHealSubWeapon::ReloadHealSubWeapon, HealCoolTime, false);
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
	UE_LOG(LogTemp, Log, TEXT("HealSubWeapon Count : %d, Current HP : %f"), CurrentHealCount, HealthComp->GetHealth());
	GetWorldTimerManager().SetTimer(HealTickTimer, this, &ASHealSubWeapon::Heal, HealTick, false);
}

void ASHealSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASHealSubWeapon::StartSubWeaponAttack()
{
	if (true == bIsCoolTime)
	{
		return;
	}

	Heal();
	bIsCoolTime = true;
}

void ASHealSubWeapon::StopSubWeaponAttack()
{

}

void ASHealSubWeapon::ReloadHealSubWeapon()
{
	bIsCoolTime = false;
	CurrentHealCount = DefaultHealCount;
}
