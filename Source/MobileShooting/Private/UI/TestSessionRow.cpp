// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSessionRow.h"
#include "UI/TestLobbyWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UTestSessionRow::Initialize()
{
	bool Result = Super::Initialize();

	SessionRowButton->OnClicked.AddDynamic(this, &UTestSessionRow::SetSelectedSessionIndex);

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