// Fill out your copyright notice in the Description page of Project Settings.


#include "SBarrier.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SHealthComponent.h"

// Sets default values
ASBarrier::ASBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	
	// 프로토타입이므로 StaticMeshComponent로 구현
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddUObject(this, &ASBarrier::OnHealthChanged);

	RootComponent = BoxComp;
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBarrier::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;

		Destroy();
	}
}

// Called every frame
void ASBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

