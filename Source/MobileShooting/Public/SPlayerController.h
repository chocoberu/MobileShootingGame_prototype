// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "SPlayerController.generated.h"

using FRequestRestartPlayerDelegate = TMulticastDelegate<void(class AController* NewPlayer)>;

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

	UFUNCTION(Client, Reliable)
	void Client_LoadPlayerStateInfo();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LoadPlayerStateInfo(const FString& NewPlayerName, int32 NewWeaponId, int32 NewSubWeaponId, int32 NewPlayerIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReadyGame(bool bReadyState);

	UFUNCTION(Client, Reliable)
	void Client_SetInputMode(bool bGameMode);

	// Delegate
	FRequestRestartPlayerDelegate RequestRestartPlayerDelegate;

protected:

	virtual void BeginPlay() override;

	void InitWidget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class URightButtonHUDWidget> RightPadButtonHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<class USPraticeMenuWidget> MenuWidgetClass;

	UPROPERTY() 
	class URightButtonHUDWidget* RightButtonHUD;

	UPROPERTY()
	class USPraticeMenuWidget* MenuWidget;

	bool bLoadPlayerState = false;
};
