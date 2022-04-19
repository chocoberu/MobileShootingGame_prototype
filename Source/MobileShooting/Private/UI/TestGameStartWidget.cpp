// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestGameStartWidget.h"
#include "UI/TestWeaponSelectWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "SGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

bool UTestGameStartWidget::Initialize()
{
	if (false == Super::Initialize())
	{
		return false;
	}

	SinglePlayButton->OnClicked.AddDynamic(this, &UTestGameStartWidget::OnClickedSinglePlayButton);
	PraticeModeButton->OnClicked.AddDynamic(this, &UTestGameStartWidget::OnClickedPraticeModeButton);
	MultiPlayButton->OnClicked.AddDynamic(this, &UTestGameStartWidget::OnClickedMultiPlayButton);
	ExitButton->OnClicked.AddDynamic(this, &UTestGameStartWidget::OnClickedExitButton);

	//WeaponSelect->SetParentWidget(this);
	WeaponSelect->OnCancelClickedDelegate.AddUObject(this, &UTestGameStartWidget::SetWidgetSwitcher, 0);

	return true;
}

void UTestGameStartWidget::OnClickedSinglePlayButton()
{
	auto TestGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		return;
	}

	TestGameInstance->SetCurrentSelectLevel(TEXT("TestBossLevel"));

	StartWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UTestGameStartWidget::OnClickedPraticeModeButton()
{
	auto TestGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		return;
	}

	TestGameInstance->SetCurrentSelectLevel(TEXT("PraticeLevel"));
	StartWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UTestGameStartWidget::OnClickedMultiPlayButton()
{
	auto TestGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		return;
	}

	TestGameInstance->SetCurrentSelectLevel(TEXT("Lobby"));
	StartWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UTestGameStartWidget::OnClickedExitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UTestGameStartWidget::SetWidgetSwitcher(const int32 Index)
{
	StartWidgetSwitcher->SetActiveWidgetIndex(Index);
}
