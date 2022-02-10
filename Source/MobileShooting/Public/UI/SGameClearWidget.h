// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USGameClearWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetGameClearWidget(bool bFlag, const int32 GameClearTime);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameOverTitle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ClearTimeText;

	UPROPERTY(meta = (BindWidget))
	class UButton* RetryButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainTitleButton;
};
