// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestSessionRow.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTestSessionRow : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetSessionName(FText NewSessionName);

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionName;


};
