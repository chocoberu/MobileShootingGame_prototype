// Fill out your copyright notice in the Description page of Project Settings.


#include "SRifleWeapon.h"
#include "SProjectile.h"

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
	if (bReloading == true)
	{
		return;
	}

	float FirstDelay = FMath::Max(LastNormalAttackTime + NormalAttackCoolTime - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(NormalAttackTimer, this, &ASRifleWeapon::NormalAttack, NormalAttackCoolTime, true, FirstDelay);
}

void ASRifleWeapon::StopNormalAttack()
{
	GetWorldTimerManager().ClearTimer(NormalAttackTimer);
}

void ASRifleWeapon::NormalAttack()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		return;
	}

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector ShotDirection = EyeRotation.Vector();

	FVector TraceEnd = EyeLocation + EyeRotation.Vector() * 1200.0f;

	ASProjectile* Bullet = GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,
		MeshComp->GetSocketLocation(WeaponMuzzleSocketName), MeshComp->GetSocketRotation(WeaponMuzzleSocketName));
	if (Bullet != nullptr)
	{
		Bullet->SetOwner(MyOwner);
	}

	// 醚舅 荐 贸府

	CurrentBulletCount--;

	UE_LOG(LogTemp, Log, TEXT("Current Bullet : %d"), CurrentBulletCount);

	if (CurrentBulletCount == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Reloading"));
		bReloading = true;
		StopNormalAttack();

		// 胆府霸捞飘 贸府
		OnReloadMontageDelegate.Broadcast();

		// 酿鸥烙 贸府
		GetWorldTimerManager().SetTimer(ReloadTimer, FTimerDelegate::CreateLambda([&]() {
			bReloading = false;
			CurrentBulletCount = DefaultBulletCount;
			}), ReloadCoolTime, false);

	}
}

void ASRifleWeapon::SkillAttack()
{

}
