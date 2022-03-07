// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSelectWeaponInventoryItem.h"
#include "UI/TestSelectWeaponInventoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UTestSelectWeaponInventoryItem::Initialize()
{
	if (false == Super::Initialize())
	{
		return false;
	}

	ItemButton->OnClicked.AddDynamic(this, &UTestSelectWeaponInventoryItem::OnClickedItemButton);

	return true;
}

void UTestSelectWeaponInventoryItem::Setup(class UTestSelectWeaponInventoryWidget* Parent, uint32 Index)
{
	if (nullptr != Parent)
	{
		ParentWidget = Parent;
	}
	ItemIndex = Index;
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

void UTestSelectWeaponInventoryItem::SetItemSelected(const bool NewFlag)
{
	bSelected = NewFlag;

	if (true == NewFlag)
	{
		ItemName->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		ItemName->SetColorAndOpacity(FLinearColor::White);
	}
}

void UTestSelectWeaponInventoryItem::OnClickedItemButton()
{
	if (nullptr == ParentWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Item's Parent is nullptr"));
		return;
	}

	ParentWidget->SetSelectedIndex(ItemIndex);
	SetItemSelected(true);
}