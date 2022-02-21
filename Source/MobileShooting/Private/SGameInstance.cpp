// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"

USGameInstance::USGameInstance()
{

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
