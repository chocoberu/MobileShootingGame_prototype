// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("Test Session");

USGameInstance::USGameInstance()
{

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

		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &USGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USGameInstance::OnFindSessionComplete);

			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (true == SessionSearch.IsValid())
			{
				UE_LOG(LogTemp, Log, TEXT("Starting Find Sessions"));
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}
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
	if (true == Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Name : %s Created"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create Session Name : %s"), *SessionName.ToString());
	}
}

void USGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (true == Success)
	{
		CreateSession();
	}
}

void USGameInstance::OnFindSessionComplete(bool Success)
{
	if (true == Success && true == SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Finish Find Sessions"));
		for (const auto& SearchResult : SessionSearch.Get()->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session Name : %s"), *SearchResult.GetSessionIdStr());
		}
	}
}

void USGameInstance::CreateSession()
{
	if (false == SessionInterface.IsValid())
	{
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 4;
	
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);

	// Debug Ç¥½Ã
	auto Engine = GetEngine();
	if (nullptr == Engine)
	{
		return;
	}
	Engine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, TEXT("Start Hosting"));
}

void USGameInstance::Host()
{
	if (false == SessionInterface.IsValid())
	{
		return;
	}

	auto ExistSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (nullptr == ExistSession)
	{
		CreateSession();
	}
	else
	{
		SessionInterface->DestroySession(SESSION_NAME);
	}
}