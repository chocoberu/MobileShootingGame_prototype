// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSessionRow.h"
#include "UI/TestLobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UTestSessionRow::Initialize()
{
	bool Result = Super::Initialize();

	SessionRowButton->OnClicked.AddDynamic(this, &UTestSessionRow::OnClickedRowButton);
	SessionRowButton->OnHovered.AddDynamic(this, &UTestSessionRow::OnHoveredRowButton);
	SessionRowButton->OnUnhovered.AddDynamic(this, &UTestSessionRow::OnUnHoveredRowButton);

	return Result;
}

void UTestSessionRow::SetParentWidget(class UTestLobbyWidget* Parent, uint32 Index)
{
	ParentWidget = Parent;
	SessionIndex = Index;
}

void UTestSessionRow::SetSessionName(FText NewSessionName)
{
	SessionName->SetText(NewSessionName);
}

void UTestSessionRow::SetSelectedSessionIndex()
{
	if (nullptr == ParentWidget)
	{
		return;
	}

	ParentWidget->SelectSessionIndex(SessionIndex);
}

void UTestSessionRow::OnClickedRowButton()
{
	SetSelectedSessionIndex();
	SetButtonStatus(true);
}

void UTestSessionRow::OnHoveredRowButton()
{
	SessionName->SetColorAndOpacity(FLinearColor::Yellow);
}

void UTestSessionRow::OnUnHoveredRowButton()
{
	SetSessionNameColor(bSelected);
}

void UTestSessionRow::SetButtonStatus(bool NewFlag)
{
	bSelected = NewFlag;

	SetSessionNameColor(bSelected);
}

void UTestSessionRow::SetSessionNameColor(bool NewFlag)
{
	if (true == bSelected)
	{
		SessionName->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		SessionName->SetColorAndOpacity(FLinearColor::White);
	}
}