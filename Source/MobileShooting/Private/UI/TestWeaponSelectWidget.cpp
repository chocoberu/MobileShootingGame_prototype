// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestWeaponSelectWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "SGameInstance.h"
#include "Kismet/GameplayStatics.h"

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

	auto TestGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		return;
	}

	int32 WeaponValue = FCString::Atoi(*WeaponID->GetText().ToString());
	int32 SubWeaponValue = FCString::Atoi(*SubWeaponID->GetText().ToString());

	TestGameInstance->SetCurrentWeaponID(WeaponValue);
	TestGameInstance->SetCurrentSubWeaponID(SubWeaponValue);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*TestGameInstance->GetCurrentSelectLevel()));
}
