// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ErrorMessageWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UErrorMessageWidget::Initialize()
{
	bool Result = Super::Initialize();

	OKButton->OnClicked.AddDynamic(this, &UErrorMessageWidget::OnClickedOkButton);

	return Result;
}

void UErrorMessageWidget::SetErrorMessage(int32 ErrorCode)
{
	// TODO : ErrorCode�� ���� Message�� �����ϵ��� �۾�
}

void UErrorMessageWidget::OnClickedOkButton()
{
	SetVisibility(ESlateVisibility::Hidden);
}