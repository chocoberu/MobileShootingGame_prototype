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

	bReloading = false;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentBulletCount = DefaultBulletCount;

	auto PlayerController = Cast<ASPlayerController>(GetOwner()->GetInstigatorController());
	if (nullptr == PlayerController)
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


void ASWeapon::SkillAttack()
{
	// 하위 클래스에서 구현
}

void ASWeapon::ReloadWeapon(void)
{
	bReloading = false;
	CurrentBulletCount = DefaultBulletCount;
	GetWorldTimerManager().ClearTimer(ReloadTimer);

	OnAttackDelegate.Broadcast();
}

