// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SGameInstance.generated.h"

using FOnFindSessionCompleteDelegate = TMulticastDelegate<void()>;

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 WeaponID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString WeaponPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float NormalAttackCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MaxBulletCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float ReloadCoolTime;

};

USTRUCT(BlueprintType)
struct FCustomSessionResult
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
	FString SessionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 CurrentPlayerNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 MaxPlayerNumber;
};

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	USGameInstance();

	virtual void Init() override;

	FStreamableManager StreamableManager;

	FString GetWeaponPath(const int32 WeaponID);
	
	FString GetSubWeaponPath(const int32 SubWeaponID);

	void GetAllWeaponData(const FString& ContextString, TArray<FWeaponData*>& OutRowArray);

	void GetAllSubWeaponData(const FString& ContextString, TArray<FWeaponData*>& OutRowArray);

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(uint32 SessionIndex);

	// TODO : 필요하다면 매개변수를 통해 QueryParam 수정하도록
	void FindSession();

	void StartSession();

	void LeaveAndDestroySession();

	TArray<FCustomSessionResult> GetSessionList() const { return SessionResultList; }

	FOnFindSessionCompleteDelegate OnFindSessionCompleteDelegate;

private:

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnStartOnlineGameComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

	// Session 관련 Delegate
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	// Delgate Handle
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;


	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* TestWeaponDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* TestSubWeaponDataTable;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	TArray<FCustomSessionResult> SessionResultList;
};
