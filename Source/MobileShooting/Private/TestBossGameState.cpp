// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBossGameState.h"
#include "Blueprint/UserWidget.h"
#include "UI/SGameTimerHUDWidget.h"
#include "UI/SGameQuestTextWidget.h"
#include "UI/SGameClearWidget.h"

ATestBossGameState::ATestBossGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

	ECurrentGameState = EGameState::E_None;
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
	GameTimerWidget->SetTimeText(static_cast<int32>(CurrentGameTime));
	UE_LOG(LogTemp, Log, TEXT("Start time : %d"), static_cast<int32>(CurrentGameTime));

	if (nullptr != GameClearWidgetClass)
	{
		GameClearWidget = CreateWidget<USGameClearWidget>(GetWorld(), GameClearWidgetClass);
	}
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

void ATestBossGameState::ShowGameClearWidget(bool bFlag)
{
	SetCurrentGameState(EGameState::E_GameOver);
	if (nullptr == GameClearWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("GameClear Widget is nullptr"));
		return;
	}
	GameClearWidget->SetGameClearWidget(bFlag, static_cast<int32>(DefaultGameTime - CurrentGameTime));
	GameClearWidget->AddToViewport();
}

