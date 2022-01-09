// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSubWeapon.h"
#include "SHealSubWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASHealSubWeapon : public ASSubWeapon
{
	GENERATED_BODY()

public:
	ASHealSubWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float HealTick;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	int32 DefaultHealCount;

	int32 CurrentHealCount;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float HealCoolTime;

	FTimerHandle HealCoolTimer;

	FTimerHandle HealTickTimer;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartSubWeaponAttack() override;

	virtual void StopSubWeaponAttack() override;
};
