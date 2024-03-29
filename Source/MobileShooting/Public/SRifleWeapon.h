// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SRifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASRifleWeapon : public ASWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASRifleWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float SkillDuration;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float SpeedupRatio;

	FTimerHandle SkillDurationTimer;

public:
	
	// Virtual fuunc 
	// 하위 클래스에서 구현
	virtual void StartNormalAttack() override;

	virtual void StopNormalAttack() override;

	virtual void NormalAttack() override;

	virtual void OnNormalAttack() override;

	virtual void StartSkillAttack() override;

	virtual void StopSkillAttack() override;

	virtual void SkillAttack() override;
};
