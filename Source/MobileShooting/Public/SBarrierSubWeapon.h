// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSubWeapon.h"
#include "SBarrierSubWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASBarrierSubWeapon : public ASSubWeapon
{
	GENERATED_BODY()
	
public:

	ASBarrierSubWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "SubWeapon")
	TSubclassOf<class ASBarrier> BarrierClass;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float BarrierReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	int32 CurrentBarrierCount;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	int32 DefaultBarrierCount;

	FTimerHandle BarrierReloadTimer;

	bool bIsReload;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartSubWeaponAttack() override;

	virtual void StopSubWeaponAttack() override;
};
