// Fill out your copyright notice in the Description page of Project Settings.

#include "TestBossGameMode.h"
#include "SPlayerController.h"
#include "SCharacter.h"
#include "STestBossCharacter.h"
#include "TestBossGameState.h"
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

	// TEST CODE
	{
		for (const auto& Boss : TActorRange<ASTestBossCharacter>(GetWorld()))
		{
			UE_LOG(LogTemp, Log, TEXT("Character Name : %s"), *Boss->GetName());
			Boss->OnBossDeadDelegate.AddUObject(this, &ATestBossGameMode::SetGameClear, true);
		}
	}
}

bool ATestBossGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	TestGameState->SetCurrentGameState(EGameState::E_GamePause);
	return Super::SetPause(PC, CanUnpauseDelegate);
}

bool ATestBossGameMode::ClearPause()
{
	TestGameState->SetCurrentGameState(EGameState::E_GamePlaying);
	return Super::ClearPause();
}

void ATestBossGameMode::SetGameClear(bool Value)
{
	TestGameState->ShowGameClearWidget(Value);
}
