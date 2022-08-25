// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SGameTimerHUDWidget.h"
#include "Components/TextBlock.h"

void USGameTimerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USGameTimerHUDWidget::SetTimeText(const int32 RemainSecond)
{
	//UE_LOG(LogTemp, Log, TEXT("Current Time : %d"), RemainSecond);
	int32 Minute = RemainSecond / 60;
	int32 Second = RemainSecond % 60;

	MinuteText->SetText(FText::FromString(FString::Printf(TEXT("%.2d"), Minute)));
	SecondText->SetText(FText::FromString(FString::Printf(TEXT("%.2d"), Second)));
}
