// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "UI/RightButtonHUDWidget.h"

void ASPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ASPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("CreateWidget"));
	RightButtonHUD = CreateWidget<URightButtonHUDWidget>(this, RightPadButtonHUDClass);
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("RightButtonHUD is nullptr"));
		return;
	}

	RightButtonHUD->AddToViewport();
	
}