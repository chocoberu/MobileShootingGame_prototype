// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestLobbyWidget.h"
#include "UI/TestSessionRow.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/CircularThrobber.h"
#include "SGameInstance.h"

UTestLobbyWidget::UTestLobbyWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	
}

bool UTestLobbyWidget::Initialize()
{
	bool Result = Super::Initialize();
	
	CreateSessionButton->OnClicked.AddDynamic(this, &UTestLobbyWidget::CreateSession);
	JoinSessionButton->OnClicked.AddDynamic(this, &UTestLobbyWidget::JoinSession);
	SessionRefreshButton->OnClicked.AddDynamic(this, &UTestLobbyWidget::FindSession);

	bIsFocusable = true;

	SGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == SGameInstance)
	{
		return false;
	}
	SessionSearchThrobber->SetVisibility(ESlateVisibility::Hidden);

	return Result;
}

void UTestLobbyWidget::CreateSession()
{
	// TODO : GameInstance의 CreateSession 호출
	if (nullptr == SGameInstance)
	{
		return;
	}
}

void UTestLobbyWidget::JoinSession()
{
	if (nullptr == SGameInstance)
	{
		return;
	}
}

void UTestLobbyWidget::FindSession()
{
	if (nullptr == SGameInstance)
	{
		return;
	}

	SGameInstance->FindSession();
	SessionSearchThrobber->SetVisibility(ESlateVisibility::Visible);

	// TODO : 콜백함수 추가 필요, 콜백 함수에서 SetSessionList 호출 필요
}

void UTestLobbyWidget::SetSessionList(TArray<FString> SessionNames)
{
	SessionScrollBox->ClearChildren();

	for (const FString& SessionName : SessionNames)
	{
		UTestSessionRow* SessionRow = CreateWidget<UTestSessionRow>(this, SessionRowClass);
		if (nullptr == SessionRow)
		{
			return;
		}

		SessionRow->SetSessionName(FText::FromString(SessionName));
		SessionScrollBox->AddChild(SessionRow);
	}
}
