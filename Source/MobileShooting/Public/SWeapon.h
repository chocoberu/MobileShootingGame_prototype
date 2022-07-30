// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

using FOnReloadMontageDelegate = TMulticastDelegate<void()>;
using FOnAttackDelegate = TMulticastDelegate<void()>;
using FOnSkillAttackDelegate = TMulticastDelegate<void()>;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	E_IDLE = 0 UMETA(DisplayName = "Idle"),
	E_RELOAD UMETA(DisplayName = "Reload"),
	E_COOLTIME UMETA(DisplayName = "CoolTime"),
};

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

	virtual void StartSkillAttack(void);

	virtual void StopSkillAttack(void);

	virtual void SkillAttack(void);

	bool IsReloading(void) const { return EWeaponState::E_RELOAD == WeaponState; }

	int32 GetCurrentBulletCount(void) { return CurrentBulletCount; }

	void ReloadWeapon(void);

	void StopReloadWeapon(void);

	float GetFirstDelay(void) const;

	float GetNormalAttackCoolTime(void) const { return NormalAttackCoolTime; }

	float GetReloadTime(void) const { return ReloadCoolTime; }

	float GetSkillCoolTime(void) const { return SkillAttackCoolTime; }

	void SetOwnerAnimInstance(class USCharacterAnimInstance* NewAnimInstance);

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

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float SkillAttackCoolTime;

	/* RPM - Bullets per minute friend by weapon */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	float LastNormalAttackTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 DefaultBulletCount;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Weapon")
	int32 CurrentBulletCount;

	bool bReloading;

	bool bReloadSkill;

	EWeaponState WeaponState;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ReloadCoolTime;

	FTimerHandle ReloadTimer;

	FTimerHandle NormalAttackTimer;

	FTimerHandle SkillAttackTimer;

	class USCharacterAnimInstance* OwnerAnimInstance;

public:
	// Delegate
	FOnReloadMontageDelegate OnReloadMontageDelegate;
	FOnAttackDelegate OnAttackDelegate;
	FOnSkillAttackDelegate OnSkillAttackDelegate;
};
