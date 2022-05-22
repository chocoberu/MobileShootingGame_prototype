// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TestBossGameState.generated.h"


UENUM(BlueprintType)
enum class ECurrentGameState : uint8
{
	E_None = 0 UMETA(DisplayName = "None"),
	E_GamePlaying UMETA(DisplayName = "GamePlaying"),
	E_GamePause UMETA(DisplayName = "GamePause"),
	E_GameOver UMETA(DisplayName = "GameOver"),
};
/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ATestBossGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ATestBossGameState();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void SetCurrentGameState(ECurrentGameState NewGameState);

	void ShowGameClearWidget(bool bFlag);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USGameQuestTextWidget> GameQuestTextWidgetClass;

	UPROPERTY()
	class USGameQuestTextWidget* GameQuestTextWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FString GameQuestString;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USGameTimerHUDWidget> GameTimerWidgetClass;

	UPROPERTY()
	class USGameTimerHUDWidget* GameTimerWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USGameClearWidget> GameClearWidgetClass;

	UPROPERTY()
	class USGameClearWidget* GameClearWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	float DefaultGameTime;

	float CurrentGameTime;

	ECurrentGameState CurrentGameState;
};
