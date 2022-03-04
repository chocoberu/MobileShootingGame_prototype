// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSelectWeaponInventoryItem.h"
#include "Components/TextBlock.h"

bool UTestSelectWeaponInventoryItem::Initialize()
{
	if (false == Super::Initialize())
	{
		return false;
	}

	return true;
}

void UTestSelectWeaponInventoryItem::SetItemId(const int32 NewItemId)
{
	ItemId = NewItemId;
}

void UTestSelectWeaponInventoryItem::SetItemName(const FString NewName)
{
	if (false == NewName.IsEmpty())
	{
		ItemName->SetText(FText::FromString(NewName));
	}
}