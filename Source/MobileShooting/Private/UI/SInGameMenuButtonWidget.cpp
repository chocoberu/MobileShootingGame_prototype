// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SInGameMenuButtonWidget.h"
#include "Components/Button.h"

void USInGameMenuButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuButton->OnClicked.AddDynamic(this, &USInGameMenuButtonWidget::OnClickedMenuButton);
}

void USInGameMenuButtonWidget::OnClickedMenuButton()
{
	// TODO : PlayerController���� ó���ϵ���?
	UE_LOG(LogTemp, Log, TEXT("Menu Button OnClick"));
}