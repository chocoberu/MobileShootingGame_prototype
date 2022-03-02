// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/TestSelectWEaponInventoryItem.h"
#include "TestSelectWeaponInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTestSelectWeaponInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual bool Initialize() override;

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InventoryName;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* GridPanel;

	UPROPERTY(EditAnywhere, Category = "WeaponType")
	bool bWeaponType;

	TArray<UTestSelectWeaponInventoryItem> WeaponItem;
};
