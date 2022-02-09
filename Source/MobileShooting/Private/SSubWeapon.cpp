// Fill out your copyright notice in the Description page of Project Settings.


#include "SSubWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SPlayerController.h"

// Sets default values
ASSubWeapon::ASSubWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComop"));

	RootComponent = SphereComp;
	MeshComp->SetupAttachment(RootComponent);

	MeshComp->SetCollisionProfileName(TEXT("Projectile"));

}

// Called when the game starts or when spawned
void ASSubWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	ReloadSubWeapon();

	auto PlayerController = Cast<ASPlayerController>(GetOwner()->GetInstigatorController());
	if (nullptr == PlayerController)
	{
		return;
	}

	PlayerController->BindSubWeaponStatusWidget(this);
}

// Called every frame
void ASSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASSubWeapon::StartSubWeaponAttack()
{
	// ���� Ŭ�������� ����
}

void ASSubWeapon::StopSubWeaponAttack()
{
	// ���� Ŭ�������� ����
}

void ASSubWeapon::SubtrackCurrentSubWeaponCount(void)
{
	--CurrentSubWeaponCount;
	OnAttackDelegate.Broadcast();
}

void ASSubWeapon::ReloadSubWeapon(void)
{
	CurrentSubWeaponCount = DefaultSubWeaponCount;
	bReload = false;
	GetWorldTimerManager().ClearTimer(ReloadTimer);

	OnAttackDelegate.Broadcast();

	UE_LOG(LogTemp, Log, TEXT("SubWeapon Reload Complete!"));
}

void ASSubWeapon::StopReloadSubWeapon(void)
{
	// GetWorldTimerManager().ClearAllTimersForObject(this); // �ش� ������δ� �ȵ� -> �Լ��� �����Լ��� �����ʿ�
	GetWorldTimerManager().ClearTimer(ReloadTimer);
}
