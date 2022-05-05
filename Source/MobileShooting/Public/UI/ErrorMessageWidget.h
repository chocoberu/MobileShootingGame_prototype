// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ErrorMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UErrorMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

	void SetErrorMessage(int32 ErrorCode);

	UFUNCTION()
	void OnClickedOkButton();

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ErrorMessageText;

	UPROPERTY(meta = (BindWidget))
	class UButton* OKButton;
};
