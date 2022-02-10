// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBossGameState.h"
#include "Blueprint/UserWidget.h"
#include "UI/SGameTimerHUDWidget.h"
#include "UI/SGameQuestTextWidget.h"

ATestBossGameState::ATestBossGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
}

void ATestBossGameState::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != GameQuestTextWidgetClass)
	{
		GameQuestTextWidget = CreateWidget<USGameQuestTextWidget>(GetWorld(), GameQuestTextWidgetClass);
		if (nullptr != GameQuestTextWidget)
		{
			if (false == GameQuestString.IsEmpty())
			{
				GameQuestTextWidget->SetText(GameQuestString);
			}
			GameQuestTextWidget->AddToViewport();
		}
	}

	if (nullptr != GameTimerWidgetClass)
	{
		GameTimerWidget = CreateWidget<USGameTimerHUDWidget>(GetWorld(), GameTimerWidgetClass);

		if (nullptr != GameTimerWidget)
		{
			GameTimerWidget->AddToViewport();
		}
	}
	CurrentGameTime = DefaultGameTime;
	ECurrentGameState = EGameState::E_GamePlaying;
}

void ATestBossGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (EGameState::E_GamePlaying == ECurrentGameState)
	{
		CurrentGameTime -= DeltaSeconds;
		GameTimerWidget->SetTimeText(static_cast<int32>(CurrentGameTime));
	}
}

void ATestBossGameState::SetCurrentGameState(EGameState NewGameState)
{
	ECurrentGameState = NewGameState;
}

