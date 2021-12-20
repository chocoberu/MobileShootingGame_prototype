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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovemetComp;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float BombDamage;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float BombAttackRadius;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float  BombTime;

	FTimerHandle BombTimer;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SubWeaponAttack() override;
};
