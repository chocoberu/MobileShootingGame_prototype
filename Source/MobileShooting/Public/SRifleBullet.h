// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SRifleBullet.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASRifleBullet : public ASProjectile
{
	GENERATED_BODY()
	
public:
	ASRifleBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
