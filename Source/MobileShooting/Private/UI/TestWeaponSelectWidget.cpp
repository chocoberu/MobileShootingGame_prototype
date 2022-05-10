// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestWeaponSelectWidget.h"
#include "UI/TestGameStartWidget.h"
#include "UI/TestSelectWeaponInventoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "SGameInstance.h"
#include "TestSaveGame.h"
#include "Kismet/GameplayStatics.h"

bool UTestWeaponSelectWidget::Initialize()
{
	if (false == Super::Initialize())
	{
		return false;
	}

	SelectButton->OnClicked.AddDynamic(this, &UTestWeaponSelectWidget::OnClickedSelectButton);
	CancelButton->OnClicked.AddDynamic(this, &UTestWeaponSelectWidget::OnClickedCancelButton);

	UE_LOG(LogTemp, Log, TEXT("UTestWeaponSelectWidget Initialize"));
	return true;
}

void UTestWeaponSelectWidget::SetParentWidget(class UTestGameStartWidget* NewParentWidget)
{
	if (nullptr == NewParentWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("ParentWidget is nullptr"));
		return;
	}

	ParentWidget = NewParentWidget;
}

void UTestWeaponSelectWidget::OnClickedSelectButton()
{
	OnClickedSelectButtonDelegate.Broadcast();
}

void UTestWeaponSelectWidget::OnClickedCancelButton()
{
	OnClickedCancelButtonDelegate.Broadcast();
}

int32 UTestWeaponSelectWidget::GetSelectedWeaponId() const
{
	if (nullptr == WeaponInventory)
	{
		return -1;
	}
	return WeaponInventory->GetSelectedWeaponId();
}

int32 UTestWeaponSelectWidget::GetSelectedSubWeaponId() const
{
	if (nullptr == SubWeaponInventory)
	{
		return -1;
	}
	return SubWeaponInventory->GetSelectedWeaponId();
}