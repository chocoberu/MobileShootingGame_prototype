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
	if (nullptr == PlayerController || false == PlayerController->IsLocalPlayerController())
	{
		return;
	}

	PlayerController->BindMainWeaponStatusWidget(this);
}

void ASWeapon::StartNormalAttack()
{
	// 하위 클래스에서 구현
}

void ASWeapon::StopNormalAttack()
{
	// 하위 클래스에서 구현
}

void ASWeapon::NormalAttack()
{
	// 하위 클래스에서 구현
}

void ASWeapon::Multicast_OnNormalAttack_Implementation()
{
	OnNormalAttack();
}

void ASWeapon::OnNormalAttack()
{

}

void ASWeapon::Client_StartSubWeaponAttack()
{
	// 하위 클래스에서 구현
}

void ASWeapon::Client_StopSubWeaponAttack()
{
	// 하위 클래스에서 구현
}

void ASWeapon::StartSkillAttack()
{
	// 하위 클래스에서 구현
}

void ASWeapon::StopSkillAttack()
{
	// 하위 클래스에서 구현
}

void ASWeapon::SkillAttack()
{
	// 하위 클래스에서 구현
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