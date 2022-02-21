// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestWeaponSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTestWeaponSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool Initialize() override;

	UFUNCTION()
	void OnSelectButtonClicked();
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* WeaponID;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* SubWeaponID;
};
