// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeamGameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UTeamGameOverWidget::SetResultText(FString Result)
{
	ResultText->SetText(FText::FromString(Result));
}

void UTeamGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnButton->OnClicked.AddDynamic(this, &UTeamGameOverWidget::OnClickedReturnButton);
}

void UTeamGameOverWidget::OnClickedReturnButton()
{
	// TODO : Lobby 화면으로 돌아가도록
}