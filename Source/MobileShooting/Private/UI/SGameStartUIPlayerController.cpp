// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SGameStartUIPlayerController.h"
#include "Blueprint/UserWidget.h"

ASGameStartUIPlayerController::ASGameStartUIPlayerController()
{

}

void ASGameStartUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	SetVirtualJoystickVisibility(false);

	if (false == IsLocalPlayerController())
	{
		return;
	}

	if (nullptr == GameStartUIClass)
	{
		return;
	}

	GameStartUI = CreateWidget<UUserWidget>(this, GameStartUIClass);
	if (nullptr == GameStartUI)
	{
		UE_LOG(LogTemp, Error, TEXT("GameStartUI is nullptr"));
		return;
	}

	GameStartUI->AddToViewport();
}
