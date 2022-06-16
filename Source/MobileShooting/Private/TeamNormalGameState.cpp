// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameState.h"
#include "TeamNormalGameMode.h"
#include "Net/UnrealNetwork.h"
#include "UI/SGameTimerHUDWidget.h"

ATeamNormalGameState::ATeamNormalGameState()
{
	GameModeClass = ATeamNormalGameMode::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;


	BlueTeamKillCount = RedTeamKillCount = 0;
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

	CurrentGamePlayTime = MaxGamePlayTime;
	BeforeGameTime = static_cast<int32>(MaxGamePlayTime);

	if (nullptr != GameTimerWidgetClass)
	{
		GameTimerWidget = CreateWidget<USGameTimerHUDWidget>(GetWorld(), GameTimerWidgetClass);

		if (nullptr != GameTimerWidget)
		{
			GameTimerWidget->AddToViewport();
		}
		GameTimerWidget->SetTimeText(BeforeGameTime);
	}
}

void ATeamNormalGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (false == HasAuthority())
	{
		return;
	}

	if (MatchState::InProgress == GetMatchState() && nullptr != GameTimerWidget)
	{
		CurrentGamePlayTime = MaxGamePlayTime - GetServerWorldTimeSeconds() + StartGameTime;
		int32 IntCurrentGamePlayTime = CurrentGamePlayTime > 0.0f ? static_cast<int32>(FMath::CeilToFloat(CurrentGamePlayTime)) : 0;
		if (BeforeGameTime > IntCurrentGamePlayTime)
		{
			UE_LOG(LogTemp, Log, TEXT("Current Game Time : %f"), CurrentGamePlayTime);
			GameTimerWidget->SetTimeText(IntCurrentGamePlayTime);
			BeforeGameTime = IntCurrentGamePlayTime;
		}

		if (CurrentGamePlayTime <= 0.0f)
		{
			CurrentGamePlayTime = 0.0f;
			BeforeGameTime = 0;
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

float ATeamNormalGameState::GetCurrentGamePlayTime() const
{
	return CurrentGamePlayTime;
}