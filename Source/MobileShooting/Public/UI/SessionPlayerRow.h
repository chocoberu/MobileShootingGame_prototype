// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionPlayerRow.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API USessionPlayerRow : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void Setup(FString NewName, bool bReady, bool bIsLocal);

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ReadyState;
};
