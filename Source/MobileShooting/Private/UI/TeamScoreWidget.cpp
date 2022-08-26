// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeamScoreWidget.h"
#include "Components/TextBlock.h"

void UTeamScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTeamScoreWidget::UpdateBlueTeamScore(const int32& CurrentBlueTeamScore)
{
	BlueTeamScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentBlueTeamScore)));
}

void UTeamScoreWidget::UpdateRedTeamScore(const int32& CurrentRedTeamScore)
{
	RedTeamScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentRedTeamScore)));
}