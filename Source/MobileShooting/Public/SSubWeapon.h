// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSubWeapon.generated.h"

using FOnAttackDelegate = TMulticastDelegate<void()>;

UENUM(BlueprintType)
enum class ESubWeaponState : uint8
{
	E_IDLE = 0 UMETA(DisplayName = "Idle"),
	E_RELOAD UMETA(DisplayName = "Reload"),
	E_COOLTIME UMETA(DisplayName = "CoolTime"),
};

UCLASS()
class MOBILESHOOTING_API ASSubWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASSubWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	int32 DefaultSubWeaponCount;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float  ReloadTime;

	int32 CurrentSubWeaponCount;

	FTimerHandle ReloadTimer;

	ESubWeaponState SubWeaponState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartSubWeaponAttack(void);

	virtual void StopSubWeaponAttack(void);

	void SubtrackCurrentSubWeaponCount(void);

	void ReloadSubWeapon(void);

	void StopReloadSubWeapon(void);

	int32 GetCurrentSubWeaponCount(void) const { return CurrentSubWeaponCount; }

	float GetReloadTime(void) const { return ReloadTime; }

	// Delegate
	FOnAttackDelegate OnAttackDelegate;
};
