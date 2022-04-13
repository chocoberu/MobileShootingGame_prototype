// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

USGameInstance::USGameInstance()
{
	CurrentWeaponID = 0;
	CurrentSubWeaponID = 1000;
}

void USGameInstance::Init()
{
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

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (nullptr != Subsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("Found OnlineSubsystem : %s"), *Subsystem->GetSubsystemName().ToString());

		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USGameInstance::OnCreateSessionComplete);
			
			// TODO : Host 쪽으로 이동
			//FOnlineSessionSettings SessionSettings;
			//SessionInterface->CreateSession(0, TEXT("Test Session"), SessionSettings);
		}
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

void USGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("Session Name %s "), *SessionName.ToString());
}