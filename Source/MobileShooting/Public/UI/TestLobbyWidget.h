// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTestLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UTestLobbyWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	UFUNCTION()
	void CreateSession();

	UFUNCTION()
	void JoinSession();

	UFUNCTION()
	void FindSession();

	void SetSessionList(TArray<FString> SessionNames);

protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateSessionButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinSessionButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* LobbyMenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SessionScrollBox;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* SessionRefreshButton;

	UPROPERTY(meta = (BindWidget))
	class UCircularThrobber* SessionSearchThrobber;

	UPROPERTY(EditAnywhere, Category = "Session")
	TSubclassOf <class UTestSessionRow> SessionRowClass;

	class USGameInstance* SGameInstance;
};
