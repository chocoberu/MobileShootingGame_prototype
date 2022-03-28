// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "SGameInstance.generated.h"


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

	void SetCurrentSelectLevel(const FString SelectLevel);

	FString GetCurrentSelectLevel() { return CurrentSelectLevel; }

	void SetCurrentWeaponID(const int32 WeaponID) { CurrentWeaponID = WeaponID; }

	void SetCurrentSubWeaponID(const int32 WeaponID) { CurrentSubWeaponID = WeaponID; }

	int32 GetCurrentWeaponID() { return CurrentWeaponID; }

	int32 GetCurrentSubWeaponID() { return CurrentSubWeaponID; }

	void GetAllWeaponData(const FString& ContextString, TArray<FWeaponData*>& OutRowArray);

	void GetAllSubWeaponData(const FString& ContextString, TArray<FWeaponData*>& OutRowArray);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* TestWeaponDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	class UDataTable* TestSubWeaponDataTable;

	// TEST CODE
	int32 CurrentWeaponID;

	int32 CurrentSubWeaponID;

	FString CurrentSelectLevel;
};
