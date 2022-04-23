// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestLobbyWidget.h"
#include "UI/TestSessionRow.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"

UTestLobbyWidget::UTestLobbyWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	
}

bool UTestLobbyWidget::Initialize()
{
	bool Result = Super::Initialize();
	
	CreateSessionButton->OnClicked.AddDynamic(this, &UTestLobbyWidget::CreateSession);
	JoinSessionButton->OnClicked.AddDynamic(this, &UTestLobbyWidget::JoinSession);

	bIsFocusable = true;

	return Result;
}

void UTestLobbyWidget::CreateSession()
{
	// TEST
	UTestSessionRow* SessionRow = CreateWidget<UTestSessionRow>(this, SessionRowClass);
	if (nullptr == SessionRow)
	{
		return;
	}

	SessionScrollBox->AddChild(SessionRow);
}

void UTestLobbyWidget::JoinSession()
{

}