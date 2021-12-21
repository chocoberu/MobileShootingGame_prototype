// Fill out your copyright notice in the Description page of Project Settings.


#include "SSubWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

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
	
}

// Called every frame
void ASSubWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASSubWeapon::StartSubWeaponAttack()
{
	// 하위 클래스에서 구현
}

void ASSubWeapon::StopSubWeaponAttack()
{
	// 하위 클래스에서 구현
}
