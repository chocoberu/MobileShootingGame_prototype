// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInGameMenuButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USInGameMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void OnClickedMenuButton();
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* MenuButton;
};
