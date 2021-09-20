// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacterAnimInstance.h"

USCharacterAnimInstance::USCharacterAnimInstance()
{

}

void USCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void USCharacterAnimInstance::PlayNormalAttack()
{
	// TODO
	if (NormalAttackMontage != nullptr)
	{
		Montage_Play(NormalAttackMontage, 1.0f);
		UE_LOG(LogTemp, Log, TEXT("NormalAttack Montage Play"));
	}
}

void USCharacterAnimInstance::PlayReload()
{
	if (ReloadMontage != nullptr)
	{
		Montage_Play(ReloadMontage, 1.0f);
		UE_LOG(LogTemp, Log, TEXT("Reload Montage Play"));
	}
}

void USCharacterAnimInstance::SetDeadAnim(bool Value)
{
	bDead = Value;
}
