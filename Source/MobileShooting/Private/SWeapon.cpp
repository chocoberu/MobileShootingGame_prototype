// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASWeapon::ASWeapon()
{	
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComop"));
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::NormalAttack()
{
	// ���� Ŭ�������� ����
}

void ASWeapon::SkillAttack()
{
	// ���� Ŭ�������� ����
}

