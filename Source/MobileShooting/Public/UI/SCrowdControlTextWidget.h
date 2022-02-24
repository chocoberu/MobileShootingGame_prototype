// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCrowdControlTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USCrowdControlTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetCCText(const FString NewCC);

	void SetHiddenInGame(bool NewFlag);

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CCText;
};
