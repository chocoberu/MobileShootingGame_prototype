// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RightButtonHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API URightButtonHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	virtual void NativeConstruct() override;

private:

	UPROPERTY()
	class UButton* MainAttackButton;

	UPROPERTY()
	class UButton* SubAttackButton;

	UFUNCTION()
	void OnMainAttackPressed();

	UFUNCTION()
	void OnMainAttackReleased();

	UFUNCTION()
	void OnSubAttackPressed();

	UFUNCTION()
	void OnSubAttackReleased();

	UPROPERTY()
	class ASPlayerController* PlayerController;

	UPROPERTY()
	class ASCharacter* PlayerCharacter;
};
