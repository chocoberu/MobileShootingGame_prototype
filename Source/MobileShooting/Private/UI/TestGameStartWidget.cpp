// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestGameStartWidget.h"
#include "UI/TestWeaponSelectWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "SGameInstance.h"
#include "TestSaveGame.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

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

	WeaponSelect->SetParentWidget(this);
	WeaponSelect->OnClickedCancelButtonDelegate.AddUObject(this, &UTestGameStartWidget::SetWidgetSwitcher, 0);
	WeaponSelect->OnClickedSelectButtonDelegate.AddUObject(this, &UTestGameStartWidget::OnSelectWeaponFinish);

	return true;
}

void UTestGameStartWidget::OnClickedSinglePlayButton()
{
	SelectedLevelName = TEXT("TestBossLevel");
	StartWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UTestGameStartWidget::OnClickedPraticeModeButton()
{

	SelectedLevelName = TEXT("PraticeLevel");
	StartWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UTestGameStartWidget::OnClickedMultiPlayButton()
{
	SelectedLevelName = TEXT("Lobby");
	UGameplayStatics::OpenLevel(GetWorld(), FName(*SelectedLevelName));
}

void UTestGameStartWidget::OnClickedExitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UTestGameStartWidget::SetWidgetSwitcher(const int32 Index)
{
	StartWidgetSwitcher->SetActiveWidgetIndex(Index);
}

FString UTestGameStartWidget::GetSelectedLevel() const
{
	return SelectedLevelName;
}

void UTestGameStartWidget::OnSelectWeaponFinish()
{
	int32 WeaponId = WeaponSelect->GetSelectedWeaponId();
	int32 SubWeaponId = WeaponSelect->GetSelectedSubWeaponId();

	// TEMP : 잘못된 값이 저장된 경우 기본값으로 하드코딩
	if (-1 == WeaponId)
	{
		WeaponId = 0;
	}	
	if (-1 == SubWeaponId)
	{
		SubWeaponId = 1000;
	}

	UTestSaveGame* NewPlayerData = NewObject<UTestSaveGame>();
	NewPlayerData->MainWeaponId = WeaponId;
	NewPlayerData->SubWeaponId = SubWeaponId;

	if (true == UGameplayStatics::SaveGameToSlot(NewPlayerData, TEXT("Test"), 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*SelectedLevelName));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Save Error"));
	}
}