// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeamGameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "SGameInstance.h"
#include "Kismet/GameplayStatics.h"

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
	// Game Start Level∑Œ ¿Ãµø
	USGameInstance* SGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == SGameInstance)
	{
		return;
	}

	SGameInstance->LeaveAndDestroySession();
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("GameStartLevel")));
}