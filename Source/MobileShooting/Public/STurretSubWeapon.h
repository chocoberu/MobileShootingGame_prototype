// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSubWeapon.h"
#include "STurretSubWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASTurretSubWeapon : public ASSubWeapon
{
	GENERATED_BODY()
	
public:

	ASTurretSubWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ASTurret> TurretClass;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float TurretReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	int32 CurrentTurretCount;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	int32 DefaultTurretCount;

	FTimerHandle TurretReloadTimer;

	bool bIsReload;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartSubWeaponAttack() override;

	virtual void StopSubWeaponAttack() override;

	void ReloadTurret();
};
