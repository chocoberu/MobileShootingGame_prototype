// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SInGameMenuButtonWidget.h"
#include "Components/Button.h"

void USInGameMenuButtonWidget::SetHiddenWidget(bool NewValue)
{
	if (true == NewValue)
	{
		MenuButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		MenuButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void USInGameMenuButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuButton->OnClicked.AddDynamic(this, &USInGameMenuButtonWidget::OnMenuButtonPressed);
}

void USInGameMenuButtonWidget::OnMenuButtonPressed()
{
	// TODO : PlayerController에서 처리하도록?
	UE_LOG(LogTemp, Log, TEXT("Menu Button OnClick"));
	OnClickedMenuButtonDelegate.Broadcast();
}