// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameState.h"
#include "TeamNormalGameMode.h"
#include "SGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "UI/SGameTimerHUDWidget.h"
#include "UI/TeamScoreWidget.h"
#include "UI/MatchStartCountDownWidget.h"

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

	CurrentCountDown = MaxCountDown;

	if (nullptr != GameTimerWidgetClass)
	{
		GameTimerWidget = CreateWidget<USGameTimerHUDWidget>(GetWorld(), GameTimerWidgetClass);

		if (nullptr != GameTimerWidget)
		{
			GameTimerWidget->AddToViewport();
		}
		GameTimerWidget->SetTimeText(BeforeGameTime);
	}

	if (nullptr != TeamScoreWidgetClass)
	{
		TeamScoreWidget = CreateWidget<UTeamScoreWidget>(GetWorld(), TeamScoreWidgetClass);
		if (nullptr != TeamScoreWidget)
		{
			TeamScoreWidget->AddToViewport();
		}
	}

	if (nullptr != MatchStartCountDownWidgetClass)
	{
		MatchStartCountDownWidget = CreateWidget<UMatchStartCountDownWidget>(GetWorld(), MatchStartCountDownWidgetClass);
	}
}

void ATeamNormalGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (nullptr == GameTimerWidget)
	{
		return;
	}

	if (MatchState::InProgress == GetMatchState())
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

	if (MatchState::WaitingPostMatch == GetMatchState())
	{
		CurrentGamePlayTime = 0.0f;
		BeforeGameTime = 0;
		GameTimerWidget->SetTimeText(0);
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

void ATeamNormalGameState::OnRep_BlueTeamKillCount()
{
	if (nullptr == TeamScoreWidget)
	{
		return;
	}

	TeamScoreWidget->AddBlueTeamScore();
}

void ATeamNormalGameState::OnRep_RedTeamKillCount()
{
	if (nullptr == TeamScoreWidget)
	{
		return;
	}

	TeamScoreWidget->AddRedTeamScore();
}

void ATeamNormalGameState::Multicast_CountDown_Implementation()
{
	// TEST CODE
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr == SGameInstance)
	{
		return;
	}

	auto Engine = SGameInstance->GetEngine();
	if (nullptr == Engine)
	{
		return;
	}

	Engine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::Printf(TEXT("%d"), CurrentCountDown));
}
