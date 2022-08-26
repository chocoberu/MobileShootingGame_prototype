// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API UTeamScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateBlueTeamScore(const int32& CurrentBlueTeamScore);

	void UpdateRedTeamScore(const int32& CurrentRedTeamScore);

protected:

	virtual void NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BlueTeamScore;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RedTeamScore;

};
