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
	CancelButton->OnClicked.AddDynamic(this, &UTestWeaponSelectWidget::OnCancelButtonClicked);

	return true;
}

void UTestWeaponSelectWidget::OnSelectButtonClicked()
{
	// TODO : EditableText에서 ID 값을 가져와서 SaveGame 
	// TODO : EditableText를 인벤토리로 변경

	auto TestGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		return;
	}

	int32 WeaponValue = FCString::Atoi(*WeaponID->GetText().ToString());
	int32 SubWeaponValue = FCString::Atoi(*SubWeaponID->GetText().ToString());

	// 임시 : Atoi 결과값이 올바르게 나오지 않을 경우 디폴트값을 저장
	if (0 == SubWeaponValue)
	{
		SubWeaponValue = 1000;
	}

	TestGameInstance->SetCurrentWeaponID(WeaponValue);
	TestGameInstance->SetCurrentSubWeaponID(SubWeaponValue);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*TestGameInstance->GetCurrentSelectLevel()));
}

void UTestWeaponSelectWidget::OnCancelButtonClicked()
{
	WeaponID->SetText(FText());
	SubWeaponID->SetText(FText());

	OnCancelClickedDelegate.Broadcast();
}
