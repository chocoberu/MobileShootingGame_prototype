// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"
#include "UI/ErrorMessageWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

const static FName SESSION_NAME = TEXT("Test Session");

USGameInstance::USGameInstance()
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &USGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &USGameInstance::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &USGameInstance::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &USGameInstance::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &USGameInstance::OnDestroySessionComplete);
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
	if (false == SessionInterface.IsValid())
	{
		return;
	}

	SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);

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

	UWorld* World = GetWorld();
	if (nullptr == World)
	{
		return;
	}
	
	// Session Start는 SessionRoom에서 처리하도록 변경
	//OnStartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
	//SessionInterface->StartSession(SessionName);

	// TEST CODE
	UGameplayStatics::OpenLevel(World, TEXT("/Game/Levels/SessionLevel"), true, "listen");
	//World->ServerTravel("/Game/Levels/SessionLevel?listen");
	//World->ServerTravel("/Game/Levels/TestBossLevel?listen");
}

void USGameInstance::OnStartOnlineGameComplete(FName SessionName, bool Success)
{
	if (false == SessionInterface.IsValid())
	{
		return;
	}

	SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);

	if (false == Success)
	{
		return;
	}

	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Levels/SessionLevel"), true, "listen");
	GetWorld()->ServerTravel("/Game/Levels/TestBossLevel?listen");
}

void USGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (false == SessionInterface.IsValid())
	{
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("OnDestroySessionComplete() called, Session Name : %s, Success : %d"), *SessionName.ToString(), Success);
	SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);

	if (true == Success)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Levels/GameStartLevel"), true);
	}
}

void USGameInstance::OnFindSessionsComplete(bool Success)
{
	if (true == Success && true == SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Finish Find Sessions"));

		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

		SessionResultList.Empty();
		for (const auto& SearchResult : SessionSearch.Get()->SearchResults)
		{
			// TODO : Current 수정 필요
			FCustomSessionResult Result;
			Result.SessionName = SearchResult.Session.OwningUserName;
			Result.MaxPlayerNumber = SearchResult.Session.SessionSettings.NumPublicConnections;
			Result.CurrentPlayerNumber = Result.MaxPlayerNumber - SearchResult.Session.NumOpenPublicConnections;

			UE_LOG(LogTemp, Warning, TEXT("Found Session Name : %s, %d / %d "), *Result.SessionName, Result.CurrentPlayerNumber, Result.MaxPlayerNumber);
			SessionResultList.Add(Result);
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

	SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	
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
	//SessionSettings.bAllowJoinInProgress = true;
	//SessionSettings.bUsesPresence = true;
	SessionSettings.bIsLANMatch = true; // TEST

	OnCreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

	ULocalPlayer* Player = GetFirstGamePlayer();
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

		OnFindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
		SessionInterface->FindSessions(*Player->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
	}
}

void USGameInstance::StartSession()
{
	if (false == SessionInterface.IsValid())
	{
		return;
	}

	OnStartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
	SessionInterface->StartSession(SESSION_NAME);
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
		// 에디터용
		//OnDestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
		SessionInterface->DestroySession(SESSION_NAME);
	}
}

void USGameInstance::Join(uint32 SessionIndex)
{
	if (false == SessionInterface.IsValid() || false == SessionSearch.IsValid())
	{
		return;
	}
	
	OnJoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

	// TEST
	ULocalPlayer* Player = GetFirstGamePlayer();
	
	if (SessionSearch->SearchResults[SessionIndex].Session.OwningUserId != Player->GetPreferredUniqueNetId())
	{
		SessionInterface->JoinSession(*Player->GetPreferredUniqueNetId(), SESSION_NAME, SessionSearch->SearchResults[SessionIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session OwningUser == Player"));
	}
}

void USGameInstance::LeaveAndDestroySession()
{
	if (false == SessionInterface.IsValid())
	{
		return;
	}

	OnDestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
	SessionInterface->DestroySession(SESSION_NAME);
}

void USGameInstance::ShowErrorMessage(uint32 ErrorCode)
{
	// TEST CODE
	// TODO : 레벨 전환 시 UMG가 사라지지 않도록 수정 필요
	if (nullptr == ErrorMessageWidget)
	{
		if (nullptr != ErrorMessageClass)
		{
			ErrorMessageWidget = CreateWidget<UErrorMessageWidget>(this, ErrorMessageClass);
			if (nullptr == ErrorMessageWidget)
			{
				UE_LOG(LogTemp, Error, TEXT("ErrorMessageWidget is nullptr"));
				return;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ErrorMessageClass is nullptr"));
			return;
		}
	}

	ErrorMessageWidget->AddToViewport();
	ErrorMessageWidget->SetVisibility(ESlateVisibility::Visible);
}