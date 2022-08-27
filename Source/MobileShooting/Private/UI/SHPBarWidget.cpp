// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SHPBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/SHealthComponent.h"

void USHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar->SetPercent(1.0f);
}

void USHPBarWidget::BindCharacterHealthComponent(USHealthComponent* NewHealthComponent)
{
	if (nullptr == NewHealthComponent)
	{
		return;
	}

	HealthCompWeakPtr = NewHealthComponent;
	
}

void USHPBarWidget::UpdateHPWidget(float Health)
{
	if (true == HealthCompWeakPtr.IsValid())
	{
		HPProgressBar->SetPercent(Health / HealthCompWeakPtr->GetDefaultHealth());
	}
}

void USHPBarWidget::SetHPBarColor(FColor Color)
{
	HPProgressBar->FillColorAndOpacity = Color;
}