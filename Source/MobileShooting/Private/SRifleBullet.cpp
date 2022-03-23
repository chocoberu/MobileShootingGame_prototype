// Fill out your copyright notice in the Description page of Project Settings.


#include "SRifleBullet.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SHealthComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASRifleBullet::ASRifleBullet()
{
	if (nullptr == ProjectileMovementComp)
	{
		return;
	}
	ProjectileMovementComp->ProjectileGravityScale = 0.0f;
	
}

void ASRifleBullet::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3.0f);
}

void ASRifleBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TEST CODE
	if (nullptr == ProjectileMovementComp)
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Current Velocity : %f"), ProjectileMovementComp->Velocity.Size());
}

void ASRifleBullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// HealthComp를 가진 액터에만 데미지 처리
	const TSet<UActorComponent*> ComponentArray = OtherActor->GetComponents();
	bool QueryResult = false;

	for (auto OtherActorComponent : ComponentArray)
	{
		FString ComponentName;
		OtherActorComponent->GetName(ComponentName);

		if (true == ComponentName.Equals(TEXT("HealthComp")))
		{
			QueryResult = true;
			break;
		}
	}

	if (false == QueryResult)
	{
		return;
	}
	
	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(AttackDamage, DamageEvent, nullptr, GetOwner());
	UE_LOG(LogTemp, Log, TEXT("Projectile Overlap %s Damage : %f"), *OtherActor->GetName(), AttackDamage);
	
	//UE_LOG(LogTemp, Log, TEXT("%s overlap"), *OtherActor->GetName());
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComp->SetVelocityInLocalSpace(FVector::ZeroVector);

	//SetActorHiddenInGame(true);
	Destroy();
}
