// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SGameQuestTextWidget.h"
#include "Components/TextBlock.h"

void USGameQuestTextWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USGameQuestTextWidget::SetText(FString NewText)
{
	QuestText->SetText(FText::FromString(NewText));
}
