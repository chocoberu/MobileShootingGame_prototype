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
	ASCharacter* SCharacter = Cast<ASCharacter>(GetOwner());
	if (nullptr == SCharacter)
	{
		return;
	}

	SetOwnerAnimInstance(SCharacter->GetSCharacterAnimInstance());
}

void ASRifleWeapon::StartNormalAttack()
{
	if(EWeaponState::E_IDLE != WeaponState)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("ASRifleWeapon::StartNormalAttack() called"));

	float FirstDelay = GetFirstDelay();
	//GetWorldTimerManager().SetTimer(NormalAttackTimer, this, &ASRifleWeapon::NormalAttack, NormalAttackCoolTime, true, FirstDelay);
	GetWorldTimerManager().SetTimer(NormalAttackTimer, FTimerDelegate::CreateLambda([&]()
		{
			if (nullptr == OwnerAnimInstance)
			{
				return;
			}
			
			OwnerAnimInstance->PlayNormalAttack();

		}), NormalAttackCoolTime, true, FirstDelay);
}

void ASRifleWeapon::StopNormalAttack()
{
	GetWorldTimerManager().ClearTimer(NormalAttackTimer);
}

void ASRifleWeapon::NormalAttack()
{
	if (EWeaponState::E_IDLE != WeaponState)
	{
		return;
	}

	ASCharacter* MyOwner = Cast<ASCharacter>(GetOwner());
	if (nullptr == MyOwner)
	{
		return;
	}

	/*FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = MyOwner;
	ASProjectile* Bullet = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
																MeshComp->GetSocketLocation(WeaponMuzzleSocketName), 
																GetOwner()->GetActorRotation(),
																SpawnParams);
	if (nullptr == Bullet)
	{
		return;
	}
	Bullet->SetGenericTeamId(MyOwner->GetGenericTeamId());*/
	
	UE_LOG(LogTemp, Log, TEXT("ASRifleWeapon::NormalAttack() called"));
	
	Multicast_OnNormalAttack();
}

void ASRifleWeapon::OnNormalAttack()
{
	UE_LOG(LogTemp, Log, TEXT("ASRifleWeapon::OnNormalAttack() called"));
	ASCharacter* MyOwner = Cast<ASCharacter>(GetOwner());
	if (nullptr == MyOwner)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = MyOwner;
	ASProjectile* Bullet = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
		MeshComp->GetSocketLocation(WeaponMuzzleSocketName),
		GetOwner()->GetActorRotation(),
		SpawnParams);

	if (nullptr == Bullet)
	{
		return;
	}
	Bullet->SetGenericTeamId(MyOwner->GetGenericTeamId());

	// 총알 수 처리
	--CurrentBulletCount;

	UE_LOG(LogTemp, Log, TEXT("Current Bullet : %d"), CurrentBulletCount);
	OnAttackDelegate.Broadcast();

	if (CurrentBulletCount <= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Reloading"));
		WeaponState = EWeaponState::E_RELOAD;
		StopNormalAttack();

		// 델리게이트 처리
		OnReloadMontageDelegate.Broadcast();

		// 쿨타임 처리
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASRifleWeapon::ReloadWeapon, ReloadCoolTime, false);
	}
}

void ASRifleWeapon::StartSkillAttack()
{
	// TODO : 코드 정리 필요
	if (true == bReloadSkill)
	{
		return;
	}

	SkillAttack();
	bReloadSkill = true; // 상태를 만들지 고민중

	GetWorldTimerManager().SetTimer(SkillDurationTimer, FTimerDelegate::CreateLambda([&]()
		{
			// TODO : 함수 정리 필요
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
		//UE_LOG(LogTemp, Error, TEXT("SpeedupRatio 값이 잘못 설정되어 있습니다."));
		return;
	}

	CharacterMovement->MaxWalkSpeed *= SpeedupRatio;

	// TODO : 이펙트 추가
}
