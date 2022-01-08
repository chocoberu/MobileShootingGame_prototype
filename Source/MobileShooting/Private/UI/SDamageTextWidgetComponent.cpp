// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SDamageTextWidgetComponent.h"
#include "UI/SDamageFontWidget.h"

void USDamageTextWidgetComponent::SetDamageText(float Damage)
{
	auto DamageWidget = Cast<USDamageFontWidget>(GetUserWidgetObject());
	if (nullptr == DamageWidget)
	{
		return;
	}
	DamageWidget->SetDamageText(Damage);

	FTimerHandle WidgetDelayTimer;
	GetWorld()->GetTimerManager().SetTimer(WidgetDelayTimer, FTimerDelegate::CreateLambda([&]()
		{
			DestroyComponent();
		}),1.0f, false);
}