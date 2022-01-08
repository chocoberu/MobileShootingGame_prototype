// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SDamageFontWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USDamageFontWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetDamageText(float Damage);
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* FadeAnimation;
};
