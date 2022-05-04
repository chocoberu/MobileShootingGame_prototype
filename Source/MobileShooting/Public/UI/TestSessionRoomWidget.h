// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestSessionRoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTestSessionRoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

	void InitPlayerRowList(uint32 Count);

	void SetPlayerRowByIndex(uint32 Index, FString PlayerName, bool bReady);

	UFUNCTION()
	void OnClickedPrevButton();

	UFUNCTION()
	void OnClickedReadyButton();

	UFUNCTION()
	void OnClickedStartButton();

	UFUNCTION()
	void OnClickedSelectWeaponButton();

	void SetStartButtonVisible(bool NewFlag);

	void OnFinishSelectWeapon();

	void OnCancelSelectWeapon();

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionName;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* BlueSideVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* RedSideVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* PrevButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ReadyButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectWeaponButton;

	UPROPERTY(meta = (BindWidget))
	class UTestWeaponSelectWidget* WeaponSelect;

	TArray<class USessionPlayerRow*> BluePlayerList;

	TArray<class USessionPlayerRow*> RedPlayerList;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USessionPlayerRow> SessionPlayerRowClass;

	uint32 MaxPlayerCount;
};
