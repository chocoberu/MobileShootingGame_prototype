// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/SHealthComponent.h"

void USHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateHPWidget();
}

void USHPBarWidget::BindCharacterHealthComponent(USHealthComponent* NewHealthComponent)
{
	if (nullptr == NewHealthComponent)
	{
		return;
	}

	HealthCompWeakPtr = NewHealthComponent;
	
}

void USHPBarWidget::UpdateHPWidget()
{
	if (true == HealthCompWeakPtr.IsValid())
	{
		HPProgressBar->SetPercent(HealthCompWeakPtr->GetHPRatio());
	}
}