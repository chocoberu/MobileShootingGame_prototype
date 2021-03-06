// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestGameStartWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTestGameStartWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

	UFUNCTION()
	void OnClickedSinglePlayButton();

	UFUNCTION()
	void OnClickedPraticeModeButton();

	UFUNCTION()
	void OnClickedMultiPlayButton();

	UFUNCTION()
	void OnClickedExitButton();

	UFUNCTION()
	void SetWidgetSwitcher(const int32 Index = 0);

	FString GetSelectedLevel() const;

	void OnSelectWeaponFinish();

protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* SinglePlayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PraticeModeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MultiPlayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* StartWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UTestWeaponSelectWidget* WeaponSelect;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Levels", meta = (AllowPrivateAccess = "true"))
	FString SelectedLevelName;

};
