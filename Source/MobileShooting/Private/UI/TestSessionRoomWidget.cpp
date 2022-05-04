// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSessionRoomWidget.h"
#include "UI/SessionPlayerRow.h"
#include "UI/TestWeaponSelectWidget.h"
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
	SelectWeaponButton->OnClicked.AddDynamic(this, &UTestSessionRoomWidget::OnClickedSelectWeaponButton);

	//WeaponSelect->SetParentWidget(this);
	WeaponSelect->OnSelectClickedDelegate.AddUObject(this, &UTestSessionRoomWidget::OnFinishSelectWeapon);
	WeaponSelect->OnCancelClickedDelegate.AddUObject(this, &UTestSessionRoomWidget::OnCancelSelectWeapon);
	WeaponSelect->SetVisibility(ESlateVisibility::Hidden);

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
		// TODO : 로컬인지 확인 필요
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

void UTestSessionRoomWidget::OnClickedSelectWeaponButton()
{
	WeaponSelect->SetVisibility(ESlateVisibility::Visible);

	// TODO : 기존 PlayerList는 어떻게 처리할지
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

void UTestSessionRoomWidget::OnFinishSelectWeapon()
{

}

void UTestSessionRoomWidget::OnCancelSelectWeapon()
{
	WeaponSelect->SetVisibility(ESlateVisibility::Hidden);
}