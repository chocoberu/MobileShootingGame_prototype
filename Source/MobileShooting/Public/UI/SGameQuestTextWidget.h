// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGameQuestTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USGameQuestTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetText(FString NewText);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestText;
};
