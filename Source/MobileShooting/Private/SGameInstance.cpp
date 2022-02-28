// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"

USGameInstance::USGameInstance()
{
	CurrentWeaponID = 0;
	CurrentSubWeaponID = 1000;
}

void USGameInstance::Init()
{
	// TODO 

	if (nullptr == TestWeaponDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon Data Table is nullptr"));
		return;
	}

	if (nullptr == TestSubWeaponDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("SubWeapon Data Table is nullptr"));
		return;
	}
}

FString USGameInstance::GetWeaponPath(const int32 WeaponID)
{
	auto WeaponData = TestWeaponDataTable->FindRow<FWeaponData>(*FString::FromInt(WeaponID), TEXT(""));
	if (nullptr != WeaponData)
	{
		return WeaponData->WeaponPath;
	}
	else
	{
		return FString();
	}
}

FString USGameInstance::GetSubWeaponPath(const int32 SubWeaponID)
{
	auto WeaponData = TestSubWeaponDataTable->FindRow<FWeaponData>(*FString::FromInt(SubWeaponID), TEXT(""));
	if (nullptr != WeaponData)
	{
		return WeaponData->WeaponPath;
	}
	else
	{
		return FString();
	}
}

void USGameInstance::SetCurrentSelectLevel(const FString SelectLevel)
{
	CurrentSelectLevel = SelectLevel;
}

void USGameInstance::GetAllWeaponData(const FString& ContextString, TArray<FWeaponData*>& OutRowArray)
{
	TestWeaponDataTable->GetAllRows(ContextString, OutRowArray);
}

void USGameInstance::GetAllSubWeaponData(const FString& ContextString, TArray<FWeaponData*>& OutRowArray)
{
	TestSubWeaponDataTable->GetAllRows(ContextString, OutRowArray);
}
