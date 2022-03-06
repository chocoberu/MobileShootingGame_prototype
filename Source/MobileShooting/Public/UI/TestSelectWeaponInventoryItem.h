// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestSelectWeaponInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTestSelectWeaponInventoryItem : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

	void Setup(class UTestSelectWeaponInventoryWidget* Parent, uint32 Index);

	int32 GetItemId() const { return ItemId; }

	void SetItemId(const int32 NewItemId);

	void SetItemName(const FString NewName);

	UFUNCTION()
	void OnClickedItemButton();

protected:

	int32 ItemId;

	uint32 ItemIndex;

	UPROPERTY(meta = (BindWidget))
	class UButton* ItemButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY()
	class UTestSelectWeaponInventoryWidget* ParentWidget;
};
