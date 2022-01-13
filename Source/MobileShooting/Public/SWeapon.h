// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

using FOnReloadMontageDelegate = TMulticastDelegate<void()>;
using FOnAttackDelegate = TMulticastDelegate<void()>;

UCLASS()
class MOBILESHOOTING_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	// Virtual fuunc 
	// 하위 클래스에서 구현
	virtual void StartNormalAttack(void);

	virtual void StopNormalAttack(void);

	virtual void NormalAttack(void);

	virtual void SkillAttack(void);

	bool IsReloading(void) const { return bReloading; }

	int32 GetCurrentBulletCount(void) { return CurrentBulletCount; }

protected:
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY()
	class ASProjectile* Projectile;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ASProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName WeaponMuzzleSocketName;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float NormalAttackCoolTime;

	/* RPM - Bullets per minute friend by weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	float LastNormalAttackTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 DefaultBulletCount;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 CurrentBulletCount;

	bool bReloading;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ReloadCoolTime;

	FTimerHandle ReloadTimer;

	FTimerHandle NormalAttackTimer;

public:
	// Delegate
	FOnReloadMontageDelegate OnReloadMontageDelegate;
	FOnAttackDelegate OnAttackDelegate;
};
