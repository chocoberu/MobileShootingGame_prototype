// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSelectWeaponInventoryWidget.h"
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

		for (auto& Item : OutWeaponDataList)
		{
			UE_LOG(LogTemp, Log, TEXT("Weapon Name : %s, Weapon ID : %d"), *Item->WeaponName, Item->WeaponID);
		}

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SubWeapon Inventory"));

		InventoryName->SetText(FText::FromString(TEXT("SubWeapon")));

		TArray<FWeaponData*> OutSubWeaponDataList;
		TestGameInstance->GetAllSubWeaponData(TEXT(""), OutSubWeaponDataList);

		for (auto& Item : OutSubWeaponDataList)
		{
			UE_LOG(LogTemp, Log, TEXT("SubWeapon Name : %s, Weapon ID : %d"), *Item->WeaponName, Item->WeaponID);
		}
	}

	return true;
}

void UTestSelectWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

}
