// Fill out your copyright notice in the Description page of Project Settings.


#include "SRifleWeapon.h"
#include "SProjectile.h"
#include "SCharacter.h"
#include "SCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

ASRifleWeapon::ASRifleWeapon()
{
	WeaponMuzzleSocketName = TEXT("Muzzle");
}

void ASRifleWeapon::BeginPlay()
{
	Super::BeginPlay();

	NormalAttackCoolTime = 60.0f / RateOfFire;

	// TEST
	SkillAttack();
}

void ASRifleWeapon::StartNormalAttack()
{
	if(EWeaponState::E_IDLE != WeaponState)
	{
		return;
	}

	float FirstDelay = GetFirstDelay();
	GetWorldTimerManager().SetTimer(NormalAttackTimer, this, &ASRifleWeapon::NormalAttack, NormalAttackCoolTime, true, FirstDelay);
}

void ASRifleWeapon::StopNormalAttack()
{
	GetWorldTimerManager().ClearTimer(NormalAttackTimer);
}

void ASRifleWeapon::NormalAttack()
{
	AActor* MyOwner = GetOwner();
	if (nullptr == MyOwner)
	{
		return;
	}

	if (nullptr != OwnerAnimInstance)
	{
		OwnerAnimInstance->PlayNormalAttack();
	}

	ASProjectile* Bullet = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
																MeshComp->GetSocketLocation(WeaponMuzzleSocketName), 
																GetOwner()->GetActorRotation());
	if (nullptr == Bullet)
	{
		return;
	}
	Bullet->SetOwner(MyOwner);

	// 醚舅 荐 贸府
	--CurrentBulletCount;

	UE_LOG(LogTemp, Log, TEXT("Current Bullet : %d"), CurrentBulletCount);
	OnAttackDelegate.Broadcast();

	if (0 >= CurrentBulletCount)
	{
		UE_LOG(LogTemp, Log, TEXT("Reloading"));
		WeaponState = EWeaponState::E_RELOAD;
		StopNormalAttack();

		// 胆府霸捞飘 贸府
		OnReloadMontageDelegate.Broadcast();

		// 酿鸥烙 贸府
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASRifleWeapon::ReloadWeapon, ReloadCoolTime, false);
	}
}

void ASRifleWeapon::StartSkillAttack()
{

}

void ASRifleWeapon::StopSkillAttack()
{

}

void ASRifleWeapon::SkillAttack()
{
	// TODO 
	ASCharacter* MyOwner = Cast<ASCharacter>(GetOwner());
	if (nullptr == MyOwner)
	{
		return;
	}
	auto CharacterMovement = MyOwner->GetCharacterMovement();
	if (nullptr == CharacterMovement)
	{
		return;
	}

	CharacterMovement->MaxWalkSpeed *= SpeedupRatio;
}
