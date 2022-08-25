// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTeamGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetResultText(FString Result);

	UFUNCTION()
	void OnClickedReturnButton();

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ResultText;

	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnButton;
};
