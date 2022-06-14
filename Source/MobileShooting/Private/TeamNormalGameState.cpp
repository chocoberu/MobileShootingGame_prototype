// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameState.h"
#include "TeamNormalGameMode.h"
#include "Net/UnrealNetwork.h"
#include "UI/SGameTimerHUDWidget.h"

ATeamNormalGameState::ATeamNormalGameState()
{
	GameModeClass = ATeamNormalGameMode::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.25f;

	GamePlayTime = 180.0f;
	BeforeGameTime = GamePlayTime;
}

void ATeamNormalGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamNormalGameState, StartGameTime);
}

void ATeamNormalGameState::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("ATeamNormalGameState::BeginPlay() called"));

	if (nullptr != GameTimerWidgetClass)
	{
		GameTimerWidget = CreateWidget<USGameTimerHUDWidget>(GetWorld(), GameTimerWidgetClass);

		if (nullptr != GameTimerWidget)
		{
			GameTimerWidget->AddToViewport();
		}
		GameTimerWidget->SetTimeText(static_cast<int32>(GamePlayTime));
	}
}

void ATeamNormalGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (false == HasAuthority())
	{
		return;
	}

	if (true == HasMatchStarted() && nullptr != GameTimerWidget)
	{
		float CurrentGameTime = GamePlayTime - GetServerWorldTimeSeconds() + StartGameTime;
		if (BeforeGameTime - CurrentGameTime >= 1.0f)
		{
			UE_LOG(LogTemp, Log, TEXT("Current Game Time : %f"), CurrentGameTime);
			GameTimerWidget->SetTimeText(static_cast<int32>(FMath::RoundToFloat(CurrentGameTime)));
			BeforeGameTime = CurrentGameTime;
		}

		if (GamePlayTime <= 0.0f)
		{
			GamePlayTime = 0.0f;
			GameTimerWidget->SetTimeText(0);
		}
	}

}

void ATeamNormalGameState::SetStartGameTime()
{
	UWorld* World = GetWorld();
	if (nullptr != World)
	{
		StartGameTime = World->GetTimeSeconds();
	}
}