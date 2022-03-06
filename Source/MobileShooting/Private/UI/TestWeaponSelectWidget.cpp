// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestWeaponSelectWidget.h"
#include "UI/TestSelectWeaponInventoryWidget.h"
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

	UE_LOG(LogTemp, Log, TEXT("UTestWeaponSelectWidget Initialize"));
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

	// TEMP : 현재 인벤토리 아이템 미선택시 임시값 하드코딩
	int32 WeaponId = WeaponInventory->GetSelectedWeaponId();
	if (-1 == WeaponId)
	{
		WeaponId = 0;
	}
	int32 SubWeaponId = SubWeaponInventory->GetSelectedWeaponId();
	if (-1 == SubWeaponId)
	{
		SubWeaponId = 1000;
	}

	TestGameInstance->SetCurrentWeaponID(WeaponId);
	TestGameInstance->SetCurrentSubWeaponID(SubWeaponId);

	UGameplayStatics::OpenLevel(GetWorld(), FName(*TestGameInstance->GetCurrentSelectLevel()));
}

void UTestWeaponSelectWidget::OnCancelButtonClicked()
{
	//WeaponID->SetText(FText());
	//SubWeaponID->SetText(FText());

	OnCancelClickedDelegate.Broadcast();
}
