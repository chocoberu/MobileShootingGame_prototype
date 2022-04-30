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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USGameInstance::OnJoinSessionComplete);

			SessionSearch = MakeShareable(new FOnlineSessionSearch());
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
	if (false == Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create Session Name : %s"), *SessionName.ToString());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Session Name : %s Created"), *SessionName.ToString());

	UEngine* Engine = GetEngine();
	if (nullptr == Engine)
	{
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Yellow, TEXT("Host Session"));

	//SessionInterface->StartSession(SessionName);

	UWorld* World = GetWorld();
	if (nullptr == World)
	{
		return;
	}

	World->ServerTravel("/Game/Levels/SessionLevel?listen");
	//World->ServerTravel("/Game/Levels/TestBossLevel?listen");
}

void USGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	// TODO : 게임이 끝난 후 세션 destroy 처리하도록 수정해야함
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
		SessionNameList.Empty();
		for (const auto& SearchResult : SessionSearch.Get()->SearchResults)
		{
			const int32 MaxConnections = SearchResult.Session.SessionSettings.NumPublicConnections;
			// TODO : 현재 입장한 인원 수를 구하도록 수정 필요
			const int32 CurrentConnections = SearchResult.Session.NumOpenPublicConnections;

			UE_LOG(LogTemp, Warning, TEXT("Found Session Name : %s, %d / %d "), *SearchResult.Session.OwningUserName, CurrentConnections, MaxConnections);
			SessionNameList.Add(SearchResult.Session.OwningUserName);
		}

		OnFindSessionCompleteDelegate.Broadcast();
	}
}

void USGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (false == SessionInterface.IsValid())
	{
		return;
	}

	auto Engine = GetEngine();
	if (nullptr == Engine)
	{
		return;
	}
	FString Address;
	if (false == SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get connect string"));
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 5.0f, FColor::Yellow, FString::Printf(TEXT("Join to %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (nullptr == PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerController is nullptr"));
		return;
	}

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
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
	//SessionSettings.bUsesPresence = true;
	SessionSettings.bIsLANMatch = true; // TEST

	// TEST
	ULocalPlayer* Player = GetFirstGamePlayer();
	//Player->GetPreferredUniqueNetId();
	
	SessionInterface->CreateSession(*Player->GetPreferredUniqueNetId(), SESSION_NAME, SessionSettings);

	// Debug 표시
	auto Engine = GetEngine();
	if (nullptr == Engine)
	{
		return;
	}
	Engine->AddOnScreenDebugMessage(0, 3.0f, FColor::Green, TEXT("Start Hosting"));
}

void USGameInstance::FindSession()
{
	if (true == SessionSearch.IsValid() && true == SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Starting Find Sessions"));

		ULocalPlayer* Player = GetFirstGamePlayer();
		if (nullptr == Player)
		{
			return;
		}

		SessionInterface->FindSessions(*Player->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
	}
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

void USGameInstance::Join(uint32 SessionIndex)
{
	if (false == SessionInterface.IsValid() || false == SessionSearch.IsValid())
	{
		return;
	}
	
	/*auto SessionSettings = SessionInterface->GetSessionSettings(SESSION_NAME);
	if (nullptr != SessionSettings)
	{
		UE_LOG(LogTemp, Log, TEXT("Session Settings is not nullptr"));
		return;
	}*/

	// TEST
	ULocalPlayer* Player = GetFirstGamePlayer();
	//Player->GetPreferredUniqueNetId();

	if (SessionSearch->SearchResults[SessionIndex].Session.OwningUserId != Player->GetPreferredUniqueNetId())
	{
		SessionInterface->JoinSession(*Player->GetPreferredUniqueNetId(), SESSION_NAME, SessionSearch->SearchResults[SessionIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session OwningUser == Player"));
	}
}