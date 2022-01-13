// Fill out your copyright notice in the Description page of Project Settings.


#include "SBarrierSubWeapon.h"
#include "SBarrier.h"

ASBarrierSubWeapon::ASBarrierSubWeapon()
{

}

void ASBarrierSubWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASBarrierSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASBarrierSubWeapon::StartSubWeaponAttack()
{
	if (true == bIsReload)
	{
		return;
	}

	auto BarrierObject = GetWorld()->SpawnActor<ASBarrier>(BarrierClass, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.0f, GetOwner()->GetActorRotation());

	if (nullptr != BarrierObject)
	{
		BarrierObject->SetOwner(GetOwner());
		--CurrentBarrierCount;
	}

	if (CurrentBarrierCount <= 0)
	{
		bIsReload = true;
		GetWorldTimerManager().SetTimer(BarrierReloadTimer, this, &ASBarrierSubWeapon::ReloadBarrier, BarrierReloadTime, false);
	}
	else
	{
		// TODO : SubWeapon CoolTime ���� 
	}
}

void ASBarrierSubWeapon::StopSubWeaponAttack()
{

}

void ASBarrierSubWeapon::ReloadBarrier()
{
	// TODO : SubWeapon ���� �ڵ带 ���� Ŭ������ �ű� �� �ִ��� Ȯ�� �ʿ�

	bIsReload = false;
	CurrentBarrierCount = DefaultBarrierCount;
	GetWorldTimerManager().ClearTimer(BarrierReloadTimer);

	//UE_LOG(LogTemp, Log, TEXT("BarrierSubWeapon Reload Complete"));
}
