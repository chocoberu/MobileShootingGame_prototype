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

	// TODO : AnimMontage���� �Ѿ� �߻� ������ ȣ���ϵ��� ����

	ASProjectile* Bullet = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
																MeshComp->GetSocketLocation(WeaponMuzzleSocketName), 
																GetOwner()->GetActorRotation());
	if (nullptr == Bullet)
	{
		return;
	}
	Bullet->SetOwner(MyOwner);

	// �Ѿ� �� ó��
	--CurrentBulletCount;

	UE_LOG(LogTemp, Log, TEXT("Current Bullet : %d"), CurrentBulletCount);
	OnAttackDelegate.Broadcast();

	if (0 >= CurrentBulletCount)
	{
		UE_LOG(LogTemp, Log, TEXT("Reloading"));
		WeaponState = EWeaponState::E_RELOAD;
		StopNormalAttack();

		// ��������Ʈ ó��
		OnReloadMontageDelegate.Broadcast();

		// ��Ÿ�� ó��
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASRifleWeapon::ReloadWeapon, ReloadCoolTime, false);
	}
}

void ASRifleWeapon::StartSkillAttack()
{
	// TODO : �ڵ� ���� �ʿ�
	if (true == bReloadSkill)
	{
		return;
	}

	SkillAttack();
	bReloadSkill = true; // ���¸� ������ �����

	GetWorldTimerManager().SetTimer(SkillDurationTimer, FTimerDelegate::CreateLambda([&]()
		{
			// TODO : �Լ� ���� �ʿ�
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

			CharacterMovement->MaxWalkSpeed /= SpeedupRatio;
			GetWorldTimerManager().SetTimer(SkillAttackTimer, this, &ASRifleWeapon::StopSkillAttack, SkillAttackCoolTime, false);
			OnSkillAttackDelegate.Broadcast();
		}), SkillDuration, false);
}

void ASRifleWeapon::StopSkillAttack()
{
	bReloadSkill = false;
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

	if (SpeedupRatio < 1.0f)
	{
		//UE_LOG(LogTemp, Error, TEXT("SpeedupRatio ���� �߸� �����Ǿ� �ֽ��ϴ�."));
		return;
	}

	CharacterMovement->MaxWalkSpeed *= SpeedupRatio;

	// TODO : ����Ʈ �߰�
}
