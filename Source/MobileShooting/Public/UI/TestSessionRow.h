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

	virtual bool Initialize() override;

	void SetParentWidget(class UTestLobbyWidget* Parent, uint32 Index);

	void SetSessionName(FText NewSessionName);

	void SetPlayerNumbers(const int32 Current, const int32 Max);

	uint32 GetSessionIndex() const { return SessionIndex; }

	UFUNCTION()
	void OnClickedRowButton();

	UFUNCTION()
	void OnHoveredRowButton();

	UFUNCTION()
	void OnUnHoveredRowButton();

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SessionName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerNumbers;

	UPROPERTY(meta = (BindWidget))
	class UButton* SessionRowButton;

	UPROPERTY()
	class UTestLobbyWidget* ParentWidget;

	uint32 SessionIndex;

	void SetSelectedSessionIndex();

	void SetButtonStatus(bool NewFlag);
	
	void SetSessionNameColor(bool NewFlag);

	bool bSelected;
};
