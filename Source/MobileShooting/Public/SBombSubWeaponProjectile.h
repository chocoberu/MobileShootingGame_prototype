// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SBombSubWeaponProjectile.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASBombSubWeaponProjectile : public ASProjectile
{
	GENERATED_BODY()
	
public:

	ASBombSubWeaponProjectile();

	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void BombAttack(AActor* OtherActor);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float BombDamage;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float BombAttackRadius;

	UPROPERTY(EditDefaultsOnly, Category = "SubWeapon")
	float  BombTime;

	FTimerHandle BombTimer;
};
