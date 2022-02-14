// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "SPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MOBILESHOOTING_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void PostInitializeComponents() override;

	virtual void OnPossess(class APawn* aPawn) override;

	void BindMainWeaponStatusWidget(class ASWeapon* MainWeapon);

	void BindSubWeaponStatusWidget(class ASSubWeapon* SubWeapon);

	void OnGamePause();

	void OnGameResume();

	void OnPlayerDead();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class URightButtonHUDWidget> RightPadButtonHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USPraticeMenuWidget> MenuWidgetClass;

	UPROPERTY() 
	class URightButtonHUDWidget* RightButtonHUD;

	UPROPERTY()
	class USPraticeMenuWidget* MenuWidget;
};
