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

	int32 GetItemId() const { return ItemId; }

	void SetItemId(const int32 NewItemId);

protected:

	int32 ItemId;
};
