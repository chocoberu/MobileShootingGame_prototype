// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWeaponStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USWeaponStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void BindMainWeapon(class ASWeapon* MainWeapon);

	void BindSubWeapon(class ASSubWeapon* SubWeapon);

	UFUNCTION()
	void SetMainWeaponText();

	UFUNCTION()
	void SetSubWeaponText();
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MainWeaponStatusText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SubWeaponStatusText;

	TWeakObjectPtr<class ASWeapon> MainWeaponWeakPtr;
	TWeakObjectPtr<class ASSubWeapon> SubWeaponWeakPtr;
};
