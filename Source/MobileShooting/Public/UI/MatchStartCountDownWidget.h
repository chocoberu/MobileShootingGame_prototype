// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchStartCountDownWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UMatchStartCountDownWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetCountDownText(int32 CountDownNumber);

protected:

	virtual void NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CountDownText;
};
