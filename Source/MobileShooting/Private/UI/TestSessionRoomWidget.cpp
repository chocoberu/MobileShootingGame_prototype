// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSessionRoomWidget.h"
#include "TestSessionGameMode.h"
#include "SGameInstance.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"

bool UTestSessionRoomWidget::Initialize()
{
	bool Result = Super::Initialize();
	
	USGameInstance* SGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == SGameInstance)
	{
		return Result;
	}

	auto Engine = SGameInstance->GetEngine();
	if (nullptr == Engine)
	{
		return Result;
	}

	ATestSessionGameMode* TestGameMode = Cast<ATestSessionGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (nullptr == TestGameMode)
	{
		Engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("GameMode is nullptr, Maybe client"));
		return Result;
	}

	auto GameSession = TestGameMode->GameSession;
	if (nullptr == GameSession)
	{
		return Result;
	}

	UE_LOG(LogTemp, Log, TEXT("Game Session : %s "), *GameSession->SessionName.ToString());

	AGameStateBase* GameState = TestGameMode->GetGameState<AGameStateBase>();
	if (nullptr == GameState)
	{
		return Result;
	}

	for (auto PlayerState : GameState->PlayerArray)
	{
		if (nullptr == PlayerState)
		{
			continue;
		}
		UE_LOG(LogTemp, Log, TEXT("Player Name : %s"), *PlayerState->GetPlayerName());
		Engine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Player %s Login"), *PlayerState->GetPlayerName()));
	}

	return Result;
}
