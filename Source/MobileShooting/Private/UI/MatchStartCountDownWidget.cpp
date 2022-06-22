// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MatchStartCountDownWidget.h"
#include "Components/TextBlock.h"

void UMatchStartCountDownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(FadeAnimation);
}

void UMatchStartCountDownWidget::SetCountDownText(int32 CountDownNumber)
{
	if (CountDownNumber > 0)
	{
		CountDownText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CountDownNumber)));
	}
	else if (CountDownNumber == 0)
	{
		CountDownText->SetText(FText::FromString(TEXT("Game Start!")));
	}
	PlayAnimation(FadeAnimation);
}