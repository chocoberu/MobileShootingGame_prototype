// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SCrowdControlTextWidget.h"
#include "Components/TextBlock.h"

void USCrowdControlTextWidget::SetCCText(const FString NewCC)
{
	// TODO : FString -> uint8 ������ ����
	CCText->SetText(FText::FromString(NewText));
}

void USCrowdControlTextWidget::SetHiddenInGame(bool NewFlag)
{
	
}