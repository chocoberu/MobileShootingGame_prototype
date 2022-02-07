// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameTimerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USGameTimerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetTimeText(const int32 RemainSecond);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MinuteText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SecondText;
};
