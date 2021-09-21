// Fill out your copyright notice in the Description page of Project Settings.


#include "SRifleBullet.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASRifleBullet::ASRifleBullet()
{

}

void ASRifleBullet::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3.0f);
}

void ASRifleBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASRifleBullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	auto Player = Cast<ACharacter>(OtherActor);
	if (Player != nullptr)
	{
		FDamageEvent DamageEvent;
		Player->TakeDamage(AttackDamage, DamageEvent, nullptr, GetOwner());
		UE_LOG(LogTemp, Log, TEXT("Projectile Overlap %s Damage : %f"), *OtherActor->GetName(), AttackDamage);
	}

	//UE_LOG(LogTemp, Log, TEXT("%s overlap"), *OtherActor->GetName());
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovemetComp->SetVelocityInLocalSpace(FVector::ZeroVector);

	//SetActorHiddenInGame(true);
	Destroy();
}
