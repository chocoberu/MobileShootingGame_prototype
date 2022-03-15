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
	CurrentHealCount = DefaultHealCount;
}

void ASHealSubWeapon::Heal()
{
	if (0 >= CurrentHealCount)
	{
		return;
	}
	--CurrentHealCount;

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
	FDamageEvent DamageEvent;
	SubWeaponOwner->TakeDamage(-HealAmount, DamageEvent, nullptr, this);

	//HealthComp->SetHealth(HealthComp->GetHealth() + HealAmount);
	UE_LOG(LogTemp, Log, TEXT("Heal Count : %d, Current HP : %f"), CurrentHealCount, HealthComp->GetHealth());
	GetWorldTimerManager().SetTimer(HealTickTimer, this, &ASHealSubWeapon::Heal, HealTick, false);
}

void ASHealSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASHealSubWeapon::StartSubWeaponAttack()
{
	if (ESubWeaponState::E_IDLE != SubWeaponState || CurrentSubWeaponCount <= 0)
	{
		return;
	}

	Heal();
	SubtrackCurrentSubWeaponCount();

	if (CurrentSubWeaponCount <= 0)
	{
		SubWeaponState = ESubWeaponState::E_RELOAD;
		GetWorldTimerManager().SetTimer(ReloadTimer, FTimerDelegate::CreateLambda([&]()
			{
				ReloadSubWeapon();
				CurrentHealCount = DefaultHealCount;
			}), ReloadTime, false);
	}
}

void ASHealSubWeapon::StopSubWeaponAttack()
{

}
