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

	TArray<FString> GetSessionNameList() const { return SessionNameList; }

	FOnFindSessionCompleteDelegate OnFindSessionCompleteDelegate;

private:

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* TestWeaponDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* TestSubWeaponDataTable;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	TArray<FString> SessionNameList;

	// TEST
	bool bEditorSessionDestroy;
};
