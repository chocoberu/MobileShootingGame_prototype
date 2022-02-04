// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SDamageFontWidget.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"

void USDamageFontWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(FadeAnimation);
}

void USDamageFontWidget::SetDamageText(float Damage)
{
	Damage = FMath::Abs<float>(Damage);
	FString DamageStr = FString::SanitizeFloat(Damage, 0);
	DamageText->SetText(FText::FromString(DamageStr));
	//DamageText->SetColorAndOpacity(FLinearColor::Yellow);
}