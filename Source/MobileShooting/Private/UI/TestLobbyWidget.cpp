// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestLobbyWidget.h"
#include "UI/TestSessionRow.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/CircularThrobber.h"
#include "Kismet/GameplayStatics.h"

UTestLobbyWidget::UTestLobbyWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	
}

bool UTestLobbyWidget::Initialize()
{
	bool Result = Super::Initialize();
	
	CreateSessionButton->OnClicked.AddDynamic(this, &UTestLobbyWidget::CreateSession);
	JoinSessionButton->OnClicked.AddDynamic(this, &UTestLobbyWidget::JoinSession);
	SessionRefreshButton->OnClicked.AddDynamic(this, &UTestLobbyWidget::FindSession);
	PrevButton->OnClicked.AddDynamic(this, &UTestLobbyWidget::OnClickedPrevButton);

	bIsFocusable = true;

	SGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == SGameInstance)
	{
		return false;
	}
	SessionSearchThrobber->SetVisibility(ESlateVisibility::Hidden);

	SGameInstance->OnFindSessionCompleteDelegate.AddUObject(this, &UTestLobbyWidget::OnFindSessionCompleted);
	
	return Result;
}

void UTestLobbyWidget::CreateSession()
{
	if (nullptr == SGameInstance)
	{
		return;
	}
	if (true == bFindSession)
	{
		SGameInstance->CancelFindSession();
		bFindSession = false;
	}

	SGameInstance->Host();
}

void UTestLobbyWidget::JoinSession()
{
	if (nullptr == SGameInstance)
	{
		return;
	}

	if (false == SelectedSessionIndex.IsSet())
	{
		return;
	}

	SGameInstance->Join(SelectedSessionIndex.GetValue());
}

void UTestLobbyWidget::FindSession()
{
	if (nullptr == SGameInstance || true == bFindSession)
	{
		return;
	}

	SGameInstance->FindSession();
	SessionSearchThrobber->SetVisibility(ESlateVisibility::Visible);

	bFindSession = true;
}

void UTestLobbyWidget::OnFindSessionCompleted()
{
	if (nullptr == SGameInstance)
	{
		return;
	}

	SetSessionList(SGameInstance->GetSessionList());
	SessionSearchThrobber->SetVisibility(ESlateVisibility::Hidden);
	bFindSession = false;
}

void UTestLobbyWidget::SetSessionList(const TArray<FCustomSessionResult> SessionList)
{
	SessionScrollBox->ClearChildren();

	uint32 Index = 0;
	for (const FCustomSessionResult& SessionResult : SessionList)
	{
		UTestSessionRow* SessionRow = CreateWidget<UTestSessionRow>(this, SessionRowClass);
		if (nullptr == SessionRow)
		{
			return;
		}

		SessionRow->SetParentWidget(this, Index++);
		SessionRow->SetSessionName(FText::FromString(SessionResult.SessionName));
		SessionRow->SetPlayerNumbers(SessionResult.CurrentPlayerNumber, SessionResult.MaxPlayerNumber);
		
		SessionScrollBox->AddChild(SessionRow);
	}
}

void UTestLobbyWidget::SelectSessionIndex(uint32 Index)
{
	SelectedSessionIndex = Index;
}

void UTestLobbyWidget::OnClickedPrevButton()
{
	if (true == bFindSession && nullptr != SGameInstance)
	{
		SGameInstance->CancelFindSession();
		bFindSession = false;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("GameStartLevel")));
}