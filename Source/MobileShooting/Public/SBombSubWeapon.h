// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSubWeapon.h"
#include "SBombSubWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASBombSubWeapon : public ASSubWeapon
{
	GENERATED_BODY()
	
public:
	ASBombSubWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PredictBombPath(class ASProjectile* Bomb, FVector StartLocation, FRotator Rotation, float InitialSpeed, float PredictTime);

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<class ASProjectile> ProjectileClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName WeaponeSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float BombMaxChargingTime;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float InitialBombSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float ChargingBombSpeed;

	FTimerHandle BombChargingTimer;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartSubWeaponAttack() override;

	virtual void StopSubWeaponAttack() override;

	virtual void OnSubWeaponAttack() override;

	virtual void Client_StartSubWeaponAttack() override;

	virtual void Client_StopSubWeaponAttack() override;
};
