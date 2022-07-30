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

	SetLifeSpan(LifeTime);
}

void ASRifleBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASRifleBullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// TODO : StaticMesh에 부딪칠때 어떻게 처리할지
	// HealthComp를 가진 액터에만 데미지 처리
	const TSet<UActorComponent*> ComponentArray = OtherActor->GetComponents();
	bool QueryResult = false;

	for (auto OtherActorComponent : ComponentArray)
	{
		if (OtherActorComponent->GetClass() == USHealthComponent::StaticClass())
		{
			QueryResult = true;
			break;
		}
		else if (OtherActorComponent->GetClass() == UStaticMeshComponent::StaticClass())
		{
			Destroy();
			return;
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

	Destroy();
}
