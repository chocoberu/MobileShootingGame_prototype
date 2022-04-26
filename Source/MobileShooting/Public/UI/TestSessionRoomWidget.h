// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestSessionRoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTestSessionRoomWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionName;


};
