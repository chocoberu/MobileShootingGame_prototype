// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionRoomPlayerController.h"
#include "TestSessionGameMode.h"
#include "UI/TestSessionRoomWidget.h"
#include "Blueprint/UserWidget.h"

void ASessionRoomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	if (false == IsLocalPlayerController() || nullptr == SessionRoomUIClass)
	{
		return;
	}

	SessionRoomWidget = CreateWidget<UTestSessionRoomWidget>(this, SessionRoomUIClass);
	if (nullptr == SessionRoomWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SessionRoom Widget is nullptr"));
		return;
	}
	SessionRoomWidget->AddToViewport();
}

void ASessionRoomPlayerController::UpdatePlayerList()
{

}

void ASessionRoomPlayerController::RequestServerPlayerList()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ATestSessionGameMode* SessionGameMode = Cast<ATestSessionGameMode>(GetWorld()->GetAuthGameMode());
		if (nullptr != SessionGameMode)
		{
			// TODO : GameMode�� PlayerListUpdate ó��
		}
	}
	else
	{
		// ���� ���� ��û �ʿ�
	}
}

void ASessionRoomPlayerController::ReadyGame()
{
}

void ASessionRoomPlayerController::StartGame()
{
}
