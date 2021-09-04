// Fill out your copyright notice in the Description page of Project Settings.


#include "SRifleBullet.h"

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
