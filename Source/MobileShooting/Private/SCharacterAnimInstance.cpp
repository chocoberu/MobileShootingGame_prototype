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
	// TODO : 애님 노티파이로 Character Attack 하도록 수정 필요
	if (nullptr != NormalAttackMontage)
	{
		Montage_Play(NormalAttackMontage, 1.0f);
		UE_LOG(LogTemp, Log, TEXT("NormalAttack Montage Play"));
	}
}

void USCharacterAnimInstance::PlayReload()
{
	if (nullptr != ReloadMontage)
	{
		Montage_Play(ReloadMontage, 1.0f);
		UE_LOG(LogTemp, Log, TEXT("Reload Montage Play"));
	}
}

void USCharacterAnimInstance::SetDeadAnim(bool Value)
{
	DeadAnimStartPosition = 0.0f;
	bDead = Value;
}

void USCharacterAnimInstance::AnimNotify_NormalAttack()
{
	OnNormalAttack.Broadcast();
}
