// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SPraticeMenuWidget.generated.h"

using FOnResumeDelegate = TMulticastDelegate<void()>;
/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USPraticeMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void OnResumeButtonPressed();

	UFUNCTION()
	void OnReturnMainMenuButtonPressed();

	FOnResumeDelegate OnResumeDelegate;
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnMainMenuButton;
};
