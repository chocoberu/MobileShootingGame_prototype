// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameState.h"
#include "TeamNormalGameMode.h"
#include "SGameInstance.h"
#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "UI/SGameTimerHUDWidget.h"
#include "UI/TeamScoreWidget.h"
#include "UI/MatchStartCountDownWidget.h"
#include "UI/TeamGameOverWidget.h"

ATeamNormalGameState::ATeamNormalGameState()
{
	GameModeClass = ATeamNormalGameMode::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

	BlueTeamKillCount = RedTeamKillCount = 0;

	StartGameTime = -1.0f;
	bAllPlayerReady = false;
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

	// UI Widget 생성
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
		if (nullptr != MatchStartCountDownWidget)
		{
			MatchStartCountDownWidget->AddToViewport();
		}
	}

	if (nullptr != GameOverWidgetClass)
	{
		GameOverWidget = CreateWidget<UTeamGameOverWidget>(GetWorld(), GameOverWidgetClass);
	}
}

void ATeamNormalGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (false == bAllPlayerReady && true == IsAllPlayerReadyState())
	{
		bAllPlayerReady = true;

		// TEST CODE : 모든 플레이어가 준비되었을 때 Match Start CountDown, 3초 이후에 카운트 다운 호출
		FTimerHandle CountDownTimer;

		GetWorldTimerManager().SetTimer(CountDownTimer, FTimerDelegate::CreateLambda([&]()
			{
				OnAllPlayerReadyDelegate.Broadcast();
			}), 3.0f, false);
	}

	if (nullptr == GameTimerWidget)
	{
		return;
	}

	SetCurrentGamePlayTime();
	SetCurrentKillCount();
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

void ATeamNormalGameState::Multicast_CountDown_Implementation(int32 CountDownNumber)
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

	Engine->AddOnScreenDebugMessage(-1, 0.5f, FColor::Yellow, FString::Printf(TEXT("%d"), CountDownNumber));

	if (nullptr == MatchStartCountDownWidget)
	{
		return;
	}

	MatchStartCountDownWidget->SetCountDownText(CountDownNumber);
}

bool ATeamNormalGameState::IsAllPlayerReadyState()
{
	int32 CurrentPlayerCount = 1;
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (nullptr != SGameInstance)
	{
		CurrentPlayerCount = SGameInstance->GetCurrentSessionPlayerCount();
	}

	bool bResult = true;
	for (auto Iter : PlayerArray)
	{
		ASPlayerState* SPlayerState = Cast<ASPlayerState>(Iter);
		if (nullptr == SPlayerState)
		{
			continue;
		}

		//UE_LOG(LogTemp, Log, TEXT("PlayerState : %s"), *SPlayerState->GetPlayerName());

		if (false == SPlayerState->IsPlayerReady())
		{
			bResult = false;
			break;
		}
	}
	return bResult;
}

void ATeamNormalGameState::Multicast_SetGameOverWidget_Implementation()
{
	if (nullptr == GameOverWidget)
	{
		return;
	}

	GameOverWidget->AddToViewport();
	
	if (BlueTeamKillCount > RedTeamKillCount)
	{
		GameOverWidget->SetResultText(TEXT("Blue Team Win"));
	}
	else if (BlueTeamKillCount < RedTeamKillCount)
	{
		GameOverWidget->SetResultText(TEXT("Red Team Win"));
	}
	else
	{
		GameOverWidget->SetResultText(TEXT("Draw"));
	}
}

void ATeamNormalGameState::SetCurrentGamePlayTime()
{
	// 게임이 시작한 경우 현재 게임 시간 설정
	if (-1.0f != StartGameTime)
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

void ATeamNormalGameState::SetCurrentKillCount()
{
	// Match가 끝난 이후에는 Kill Count를 추가하지 않음
	if (MatchState::WaitingPostMatch == GetMatchState())
	{
		return;
	}

	int Blue = 0, Red = 0;
	for (auto Iter : PlayerArray)
	{
		ASPlayerState* PlayerState = Cast<ASPlayerState>(Iter);
		if (nullptr == PlayerState)
		{
			continue;
		}

		if (PlayerState->GetTeamNumber() == 0)
		{
			Blue += PlayerState->GetKillScore();
		}
		else
		{
			Red += PlayerState->GetKillScore();
		}
	}

	if (Blue > BlueTeamKillCount)
	{
		BlueTeamKillCount = Blue;
		TeamScoreWidget->AddBlueTeamScore();
	}
	if (Red > RedTeamKillCount)
	{
		RedTeamKillCount = Red;
		TeamScoreWidget->AddRedTeamScore();
	}
}
