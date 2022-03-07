// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSelectWeaponInventoryWidget.h"
#include "UI/TestSelectWEaponInventoryItem.h"
#include "SGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

bool UTestSelectWeaponInventoryWidget::Initialize()
{
	if (false == Super::Initialize())
	{
		return false;
	}

	auto World = GetWorld();
	if (nullptr == World)
	{
		return true;
	}

	auto TestGameInstance = Cast<USGameInstance>(World->GetGameInstance());
	if (nullptr == TestGameInstance)
	{
		return true;
	}

	if (true == bWeaponType)
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon Inventory"));

		InventoryName->SetText(FText::FromString(TEXT("Weapon")));

		TArray<FWeaponData*> OutWeaponDataList;
		TestGameInstance->GetAllWeaponData(TEXT(""), OutWeaponDataList);

		if (nullptr == InventoryItemClass)
		{
			return true;
		}

		int32 Column = 0;
		for (auto& Item : OutWeaponDataList)
		{
			auto InventoryItem = CreateWidget<UTestSelectWeaponInventoryItem>(World, InventoryItemClass);
			if (nullptr == InventoryItem)
			{
				continue;
			}

			WeaponItemList.Add(InventoryItem);
			InventoryItem->Setup(this, Column);
			InventoryItem->SetItemId(Item->WeaponID);
			InventoryItem->SetItemName(Item->WeaponName);
			
			GridPanel->AddChildToUniformGrid(InventoryItem, 0, Column++);

			UE_LOG(LogTemp, Log, TEXT("Weapon Name : %s, Weapon ID : %d"), *Item->WeaponName, Item->WeaponID);
		}

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SubWeapon Inventory"));

		InventoryName->SetText(FText::FromString(TEXT("SubWeapon")));

		TArray<FWeaponData*> OutSubWeaponDataList;
		TestGameInstance->GetAllSubWeaponData(TEXT(""), OutSubWeaponDataList);

		int32 Column = 0;
		for (auto& Item : OutSubWeaponDataList)
		{
			auto InventoryItem = CreateWidget<UTestSelectWeaponInventoryItem>(World, InventoryItemClass);
			if (nullptr == InventoryItem)
			{
				continue;
			}
			WeaponItemList.Add(InventoryItem);
			InventoryItem->Setup(this, Column);
			InventoryItem->SetItemId(Item->WeaponID);
			InventoryItem->SetItemName(Item->WeaponName);
			
			GridPanel->AddChildToUniformGrid(InventoryItem, Column++, 0);

			UE_LOG(LogTemp, Log, TEXT("SubWeapon Name : %s, Weapon ID : %d"), *Item->WeaponName, Item->WeaponID);
		}
	}

	return true;
}

void UTestSelectWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

uint32 UTestSelectWeaponInventoryWidget::GetSelectedIndex()
{
	if (true == SelectedIndex.IsSet())
	{
		return SelectedIndex.GetValue();
	}
	else
	{
		return -1;
	}
}

void UTestSelectWeaponInventoryWidget::SetSelectedIndex(uint32 NewIndex)
{
	if (true == SelectedIndex.IsSet())
	{
		WeaponItemList[SelectedIndex.GetValue()]->SetItemSelected(false);
	}
	SelectedIndex = NewIndex;
}

int32 UTestSelectWeaponInventoryWidget::GetSelectedWeaponId()
{
	if (true == SelectedIndex.IsSet())
	{
		//UE_LOG(LogTemp, Log, TEXT("Index Get Value : %d"), SelectedIndex.GetValue());
		return WeaponItemList[SelectedIndex.GetValue()]->GetItemId();
	}
	else
	{
		return -1;
	}
}