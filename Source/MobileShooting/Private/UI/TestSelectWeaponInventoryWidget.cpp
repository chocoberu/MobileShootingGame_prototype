// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestSelectWeaponInventoryWidget.h"
#include "SGameInstance.h"
#include "..\..\Public\UI\TestSelectWeaponInventoryWidget.h"

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

	TArray<FWeaponData*> OutWeaponDataList;
	TestGameInstance->GetAllWeaponData(TEXT(""), OutWeaponDataList);
	TArray<FWeaponData*> OutSubWeaponDataList;
	TestGameInstance->GetAllSubWeaponData(TEXT(""), OutSubWeaponDataList);

	for (auto& Item : OutWeaponDataList)
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon Name : %s, Weapon ID : %d"), *Item->WeaponName, Item->WeaponID);
	}

	for (auto& Item : OutSubWeaponDataList)
	{
		UE_LOG(LogTemp, Log, TEXT("SubWeapon Name : %s, Weapon ID : %d"), *Item->WeaponName, Item->WeaponID);
	}
	

	return true;
}

void UTestSelectWeaponInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

}
