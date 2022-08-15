// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "SPlayerController.h"

// Sets default values
ASWeapon::ASWeapon()
{	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComop"));
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	RootComponent = MeshComp;

	//bReloading = false;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentBulletCount = DefaultBulletCount;
	WeaponState = EWeaponState::E_IDLE;

	auto PlayerController = Cast<ASPlayerController>(GetOwner()->GetInstigatorController());
	if (nullptr == PlayerController)
	{
		return;
	}

	PlayerController->BindMainWeaponStatusWidget(this);
}

void ASWeapon::StartNormalAttack()
{
	// ���� Ŭ�������� ����
}

void ASWeapon::StopNormalAttack()
{
	// ���� Ŭ�������� ����
}

void ASWeapon::NormalAttack()
{
	// ���� Ŭ�������� ����
}

void ASWeapon::Multicast_OnNormalAttack_Implementation()
{
	OnNormalAttack();
}

void ASWeapon::OnNormalAttack()
{

}

void ASWeapon::StartSkillAttack()
{
	// ���� Ŭ�������� ����
}

void ASWeapon::StopSkillAttack()
{
	// ���� Ŭ�������� ����
}

void ASWeapon::SkillAttack()
{
	// ���� Ŭ�������� ����
}

void ASWeapon::ReloadWeapon(void)
{
	WeaponState = EWeaponState::E_IDLE;
	CurrentBulletCount = DefaultBulletCount;
	GetWorldTimerManager().ClearTimer(ReloadTimer);

	OnAttackDelegate.Broadcast();
}

void ASWeapon::StopReloadWeapon(void)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	//GetWorldTimerManager().ClearTimer(ReloadTimer);
}

float ASWeapon::GetFirstDelay(void) const
{
	return FMath::Max(LastNormalAttackTime + NormalAttackCoolTime - GetWorld()->TimeSeconds, 0.0f);
}

void ASWeapon::SetOwnerAnimInstance(USCharacterAnimInstance* NewAnimInstance)
{
	if (nullptr == NewAnimInstance)
	{
		return;
	}
	OwnerAnimInstance = NewAnimInstance;
}