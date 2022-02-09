// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBossGameMode.h"
#include "SPlayerController.h"
#include "SCharacter.h"
#include "STestBossCharacter.h"
#include "TestBossGameState.h"
#include "Blueprint/UserWidget.h"
#include "UI/SGameTimerHUDWidget.h"
#include "UI/SGameQuestTextWidget.h"
#include "EngineUtils.h"

ATestBossGameMode::ATestBossGameMode()
{
	DefaultPawnClass = ASCharacter::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
	GameStateClass = ATestBossGameState::StaticClass();
}

void ATestBossGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TestGameState = Cast<ATestBossGameState>(GameState);
}

void ATestBossGameMode::StartPlay()
{
	Super::StartPlay();
}

void ATestBossGameMode::BeginPlay()
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

	// TEST CODE
	{
		for (const auto& Boss : TActorRange<ASTestBossCharacter>(GetWorld()))
		{
			UE_LOG(LogTemp, Log, TEXT("Character Name : %s"), *Boss->GetName());
			Boss->OnBossDeadDelegate.AddUObject(this, &ATestBossGameMode::SetGameClear, true);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Start Timer"));
	StartTimer();
}

void ATestBossGameMode::StartTimer()
{
	CurrentRemainTime = DefaultRemainTime;

	if (nullptr != GameTimerWidgetClass)
	{
		GameTimerWidget = CreateWidget<USGameTimerHUDWidget>(GetWorld(), GameTimerWidgetClass);

		if (nullptr != GameTimerWidget)
		{
			GameTimerWidget->SetTimeText(CurrentRemainTime);
			GameTimerWidget->AddToViewport();
		}
		GetWorldTimerManager().SetTimer(GameTimer, this, &ATestBossGameMode::UpdateTimeCount, 1.0f, true);
	}
}

void ATestBossGameMode::UpdateTimeCount()
{
	--CurrentRemainTime;
	//UE_LOG(LogTemp, Log, TEXT("Time : %d"), CurrentRemainTime);

	GameTimerWidget->SetTimeText(CurrentRemainTime);
	if (0 == CurrentRemainTime)
	{
		GetWorldTimerManager().ClearTimer(GameTimer);
		SetGameClear(false);
	}
}

void ATestBossGameMode::SetGameClear(bool Value)
{
	// Game Clear
	if (true == Value)
	{
		UE_LOG(LogTemp, Log, TEXT("Game Clear"));
		// TODO : UI 코드 추가
	}
	// Game Over
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Game Over"));
		// TODO : UI 코드 추가
	}
}
