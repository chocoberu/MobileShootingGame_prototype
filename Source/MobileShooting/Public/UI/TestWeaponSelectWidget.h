// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestWeaponSelectWidget.generated.h"

using FOnClickedCancelButtonDelegate = TMulticastDelegate<void()>;
using FOnClickedSelectButtonDelegate = TMulticastDelegate<void()>;
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
	void OnClickedSelectButton();

	UFUNCTION()
	void OnClickedCancelButton();

	void SetParentWidget(class UTestGameStartWidget* NewParentWidget);

	int32 GetSelectedWeaponId() const;

	int32 GetSelectedSubWeaponId() const;

	FOnClickedCancelButtonDelegate OnClickedCancelButtonDelegate;

	FOnClickedSelectButtonDelegate OnClickedSelectButtonDelegate;
	
protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SelectButton;

	UPROPERTY(meta = (BindWidget))
	class UTestSelectWeaponInventoryWidget* WeaponInventory;

	UPROPERTY(meta = (BindWidget))
	class UTestSelectWeaponInventoryWidget* SubWeaponInventory;

	class UTestGameStartWidget* ParentWidget;

};
