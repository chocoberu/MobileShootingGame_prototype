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
	UE_LOG(LogTemp, Log, TEXT("BindCharacterHealthComponent() Call"));
	if (nullptr == NewHealthComponent)
	{
		return;
	}

	HealthCompWeakPtr = NewHealthComponent;
	
}

void USHPBarWidget::UpdateHPWidget()
{
	UE_LOG(LogTemp, Log, TEXT("UpdateHPWidget() Call"));
	if (true == HealthCompWeakPtr.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("%f HP Ratio"), HealthCompWeakPtr->GetHPRatio());
		HPProgressBar->SetPercent(HealthCompWeakPtr->GetHPRatio());
	}
	else
	{

	}
}