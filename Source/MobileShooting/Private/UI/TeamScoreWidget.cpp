// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeamScoreWidget.h"
#include "Components/TextBlock.h"

void UTeamScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentBlueTeamScore = CurrentRedTeamScore = 0;
}

void UTeamScoreWidget::AddBlueTeamScore()
{
	CurrentBlueTeamScore++;
	BlueTeamScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentBlueTeamScore)));
}

void UTeamScoreWidget::AddRedTeamScore()
{
	CurrentRedTeamScore++;
	RedTeamScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentRedTeamScore)));
}