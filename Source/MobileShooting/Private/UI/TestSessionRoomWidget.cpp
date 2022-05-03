// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSessionRoomWidget.h"
#include "UI/SessionPlayerRow.h"
#include "TestSessionGameMode.h"
#include "SessionRoomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

bool UTestSessionRoomWidget::Initialize()
{
	bool Result = Super::Initialize();
	InitPlayerRowList(4);

	PrevButton->OnClicked.AddDynamic(this, &UTestSessionRoomWidget::OnClickedPrevButton);
	ReadyButton->OnClicked.AddDynamic(this, &UTestSessionRoomWidget::OnClickedReadyButton);
	StartButton->OnClicked.AddDynamic(this, &UTestSessionRoomWidget::OnClickedStartButton);
	
	return Result;
}

void UTestSessionRoomWidget::InitPlayerRowList(uint32 Count)
{
	MaxPlayerCount = Count;

	for (uint32 Index = 0; Index < MaxPlayerCount; Index++)
	{
		USessionPlayerRow* PlayerRow = CreateWidget<USessionPlayerRow>(this, SessionPlayerRowClass);

		if (nullptr == PlayerRow)
		{
			return;
		}

		if (Index % 2 == 0)
		{
			BlueSideVerticalBox->AddChildToVerticalBox(PlayerRow);
			BluePlayerList.Add(PlayerRow);
		}
		else
		{
			RedSideVerticalBox->AddChildToVerticalBox(PlayerRow);
			RedPlayerList.Add(PlayerRow);
		}

		PlayerRow->Setup(TEXT("Empty"), false, false);
	}
}

void UTestSessionRoomWidget::SetPlayerRowByIndex(uint32 Index, FString PlayerName, bool bReady)
{
	if (Index >= MaxPlayerCount)
	{
		return;
	}

	if (Index % 2 == 0)
	{
		// TODO : �������� Ȯ�� �ʿ�
		BluePlayerList[Index / 2]->Setup(PlayerName, bReady, false);
	}
	else
	{
		RedPlayerList[Index / 2]->Setup(PlayerName, bReady, false);
	}
}

void UTestSessionRoomWidget::OnClickedPrevButton()
{
	ASessionRoomPlayerController* SPlayerController = GetOwningPlayer<ASessionRoomPlayerController>();
	if (nullptr == SPlayerController)
	{
		return;
	}

	SPlayerController->LeaveSession();
}

void UTestSessionRoomWidget::OnClickedReadyButton()
{
	ASessionRoomPlayerController* SPlayerController = GetOwningPlayer<ASessionRoomPlayerController>();
	if (nullptr == SPlayerController)
	{
		return;
	}
	SPlayerController->ChangeReadyState();
}

void UTestSessionRoomWidget::OnClickedStartButton()
{
	ASessionRoomPlayerController* SPlayerController = GetOwningPlayer<ASessionRoomPlayerController>();
	if (nullptr == SPlayerController)
	{
		return;
	}
	SPlayerController->StartGame();
}

void UTestSessionRoomWidget::SetStartButtonVisible(bool NewFlag)
{
	if (true == NewFlag)
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		StartButton->SetVisibility(ESlateVisibility::Hidden);
	}
}