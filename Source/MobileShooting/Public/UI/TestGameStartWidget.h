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
	void OnClickedExitButton();

protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* SinglePlayButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PraticeModeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* StartWidgetSwitcher;

};
