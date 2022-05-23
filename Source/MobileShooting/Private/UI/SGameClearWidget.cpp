// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/SGameClearWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


void USGameClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//RetryButton->OnPressed.AddDynamic(this, &USGameClearWidget::NativeConstruct);
}

void USGameClearWidget::SetGameClearWidget(bool bGameClear, const int32 GameClearTime)
{
	// Game Clear
	if(true == bGameClear)
	{
		GameOverTitle->SetText(FText::FromString(TEXT("GAME CLEAR")));
		ClearTimeText->SetText(FText::FromString(FString::Printf(TEXT("CLEAR TIME  %.2d : %.2d"), GameClearTime / 60, GameClearTime % 60)));
	}
	// Game Over
	else
	{
		GameOverTitle->SetText(FText::FromString(TEXT("GAME OVER")));
		ClearTimeText->SetVisibility(ESlateVisibility::Hidden);
	}

}