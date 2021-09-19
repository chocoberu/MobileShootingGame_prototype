// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	USCharacterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void PlayNormalAttack();

	virtual void PlayReload();

protected:

	// Montage

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* NormalAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	UAnimMontage* ReloadMontage;
};
