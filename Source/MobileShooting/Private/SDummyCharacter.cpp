// Fill out your copyright notice in the Description page of Project Settings.


#include "SDummyCharacter.h"
#include "Components/SHealthComponent.h"

// Sets default values
ASDummyCharacter::ASDummyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddUObject(this, &ASDummyCharacter::OnHealthChanged);

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));


}

// Called when the game starts or when spawned
void ASDummyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASDummyCharacter::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// TODO : HealthComponent에 데미지 UI 처리를 할지, OnHealthChanged()에서 UI 처리를 할지 결정 필요
}

// Called every frame
void ASDummyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASDummyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

