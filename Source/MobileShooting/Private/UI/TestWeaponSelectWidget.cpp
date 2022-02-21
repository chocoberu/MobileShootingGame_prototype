// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestWeaponSelectWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UTestWeaponSelectWidget::Initialize()
{
	if (false == Super::Initialize())
	{
		return false;
	}

	SelectButton->OnClicked.AddDynamic(this, &UTestWeaponSelectWidget::OnSelectButtonClicked);

	return true;
}

void UTestWeaponSelectWidget::OnSelectButtonClicked()
{
	// TODO : EditableText���� ID ���� �����ͼ� SaveGame 
	// TODO : EditableText�� �κ��丮�� ����
}
