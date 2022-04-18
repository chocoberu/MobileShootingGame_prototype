// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestWeaponSelectWidget.h"
#include "UI/TestSelectWeaponInventoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "SGameInstance.h"
#include "TestSaveGame.h"
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
	auto TestGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		return;
	}

	// TEMP : ���� �κ��丮 ������ �̼��ý� �ӽð� �ϵ��ڵ�
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
	
	// ���� ������ weapon id, subweapon id�� gameinstance�� ���� -> SaveGame���� PlayerState�� ���� �ʿ�
	UTestSaveGame* NewPlayerData = NewObject<UTestSaveGame>();
	NewPlayerData->MainWeaponId = WeaponId;
	NewPlayerData->SubWeaponId = SubWeaponId;

	if (true == UGameplayStatics::SaveGameToSlot(NewPlayerData, TEXT("Test"), 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(*TestGameInstance->GetCurrentSelectLevel()));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Save Error"));
	}
	//TestGameInstance->SetCurrentWeaponID(WeaponId);
	//TestGameInstance->SetCurrentSubWeaponID(SubWeaponId);
}

void UTestWeaponSelectWidget::OnCancelButtonClicked()
{
	OnCancelClickedDelegate.Broadcast();
}
