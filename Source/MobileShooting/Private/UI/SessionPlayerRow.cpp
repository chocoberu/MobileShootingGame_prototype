// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SessionPlayerRow.h"
#include "Components/TextBlock.h"

void USessionPlayerRow::Setup(FString NewName, bool bReady, bool bIsLocal)
{
	if (true == bIsLocal)
	{
		PlayerName->SetText(FText::FromString(TEXT("ME")));
	}
	else
	{
		PlayerName->SetText(FText::FromString(NewName));
	}
	
	if (true == bReady)
	{
		ReadyState->SetText(FText::FromString(TEXT("READY")));
	}
	else
	{
		ReadyState->SetText(FText::FromString(TEXT("NOT READY")));
	}
}