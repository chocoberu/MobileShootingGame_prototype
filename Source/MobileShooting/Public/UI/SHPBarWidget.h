// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USHPBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHPWidget(float Health);

	void BindCharacterHealthComponent(class USHealthComponent* NewHealthComponent);
	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPProgressBar;

	TWeakObjectPtr<class USHealthComponent> HealthCompWeakPtr;
};
