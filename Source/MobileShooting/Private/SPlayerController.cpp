// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SCharacter.h"
#include "SGameInstance.h"
#include "TestSaveGame.h"
#include "UI/RightButtonHUDWidget.h"
#include "UI/SPraticeMenuWidget.h"
#include "Kismet/GameplayStatics.h"

void ASPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::OnPossess() call"));

	// TEST CODE
	if (false == bLoadPlayerState)
	{
		Client_LoadPlayerStateInfo();
	}

	ASCharacter* PlayerCharacter = Cast<ASCharacter>(aPawn);
	if (nullptr == PlayerCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::OnPossess(), PlayerCharacter is nullptr"));
		return;
	}
	
	PlayerCharacter->LoadWeapon();
}

void ASPlayerController::BindMainWeaponStatusWidget(ASWeapon* MainWeapon)
{
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("ASPlayerController::BindMainWeaponStatusWidget(), RightButtonHUD is nullptr"));
		return;
	}
	RightButtonHUD->BindMainWeapon(MainWeapon);
}

void ASPlayerController::BindSubWeaponStatusWidget(ASSubWeapon* SubWeapon)
{
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("ASPlayerController::BindSubWeaponStatusWidget(), RightButtonHUD is nullptr"));
		return;
	}
	RightButtonHUD->BindSubWeapon(SubWeapon);
}

void ASPlayerController::OnGamePause()
{
	if (nullptr == MenuWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::OnGamePause(), Menu Widget is nullptr"));
		return;
	}
	SetPause(true);
	RightButtonHUD->SetVisibleWeaponStatusAnimation(true);

	MenuWidget->AddToViewport();
	bShowMouseCursor = true;

	// TEST CODE
	FInputModeUIOnly Mode;
	SetInputMode(Mode);
}

void ASPlayerController::OnGameResume()
{
	if (nullptr == MenuWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::OnGameResume(), Menu Widget is nullptr"));
		return;
	}
	SetPause(false);
	RightButtonHUD->SetVisibleWeaponStatusAnimation(false);

	MenuWidget->RemoveFromViewport();
	bShowMouseCursor = false;

	// TEST CODE
	FInputModeGameAndUI Mode;
	SetInputMode(Mode);
}

void ASPlayerController::OnPlayerDead()
{
	// Stop All Widget Animations
	if (nullptr == RightButtonHUD)
	{
		return;
	}
	RightButtonHUD->StopAllWidgetAnimations();

	// TODO : SCharacter ��� �� �۾� �߰�
}

void ASPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::PostInitializeComponents() called"));
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BeginPlay() call"));

	// TEST CODE
	if (false == IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Log, TEXT("ASPlayerController::BeginPlay(), No Local Controller"));
		return;
	}
	
	// TODO : �Ϻ� �÷������� HUD�� Weapon Bind�� ����� ���� ���� (HUD == nullptr�̿���)
	// OnPossess()�� Ŭ���̾�Ʈ���� ȣ����� �����Ƿ� �ٸ� ������ ���� �ʿ�
	/*if (nullptr != RightPadButtonHUDClass)
	{
		RightButtonHUD = CreateWidget<URightButtonHUDWidget>(this, RightPadButtonHUDClass);
	}
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("RightButtonHUD is nullptr"));
		return;
	}

	if (nullptr != MenuWidgetClass)
	{
		MenuWidget = CreateWidget<USPraticeMenuWidget>(this, MenuWidgetClass);
	}
	if (nullptr != MenuWidget)
	{
		MenuWidget->OnResumeDelegate.AddUObject(this, &ASPlayerController::OnGameResume);
	}*/

	FString CurrentPlatform = UGameplayStatics::GetPlatformName();
	// �ӽ� �ּ� ó��
	//if (0 == CurrentPlatform.Compare(TEXT("IOS")) || 0 == CurrentPlatform.Compare(TEXT("Android")))
	{
		SetVirtualJoystickVisibility(true);
		if (nullptr == RightButtonHUD)
		{
			return;
		}
		RightButtonHUD->AddToViewport();
		if (nullptr == MenuWidget)
		{
			RightButtonHUD->SetHiddenMenuButton(true);
		}
	}
}

void ASPlayerController::InitWidget()
{
	if (nullptr != RightPadButtonHUDClass)
	{
		RightButtonHUD = CreateWidget<URightButtonHUDWidget>(this, RightPadButtonHUDClass);
	}
	if (nullptr == RightButtonHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("RightButtonHUD is nullptr"));
		return;
	}

	if (nullptr != MenuWidgetClass)
	{
		MenuWidget = CreateWidget<USPraticeMenuWidget>(this, MenuWidgetClass);
	}
	if (nullptr != MenuWidget)
	{
		MenuWidget->OnResumeDelegate.AddUObject(this, &ASPlayerController::OnGameResume);
	}
}

void ASPlayerController::Client_LoadPlayerStateInfo_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::Client_LoadPlayerStateInfo() called"));

	// TEST CODE
	InitWidget();
	
	USGameInstance* SGameInstance = Cast<USGameInstance>(GetGameInstance());
	if (nullptr != SGameInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("Current Player Name : %s"), *SGameInstance->GetCurrentPlayerName());
	}
	
	FString PlayerName = SGameInstance != nullptr ? SGameInstance->GetCurrentPlayerName() : TEXT("Player0");
	UTestSaveGame* SaveGame = Cast<UTestSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerName, 0));
	if (nullptr == SaveGame)
	{
		UE_LOG(LogTemp, Error, TEXT("ASPlayerController::Client_LoadPlayerStateInfo(), Failed to LoadGame"));
		return;
	}
	else
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
			if (nullptr == SPlayerState)
			{
				UE_LOG(LogTemp, Error, TEXT("ASPlayerState is nullptr"))
			}
			else
			{
				SPlayerState->SetPlayerName(PlayerName);
				SPlayerState->SetWeaponId(SaveGame->MainWeaponId);
				SPlayerState->SetSubWeaponId(SaveGame->SubWeaponId);
				SPlayerState->SetTeamNumber(SaveGame->TeamNum);
			}
			bLoadPlayerState = true;
			RequestRestartPlayerDelegate.Broadcast(this);
		}
		else
		{
			Server_LoadPlayerStateInfo(PlayerName, SaveGame->MainWeaponId, SaveGame->SubWeaponId, SaveGame->TeamNum);
		}
	}
}

void ASPlayerController::Server_LoadPlayerStateInfo_Implementation(const FString& NewPlayerName, int32 NewWeaponId, int32 NewSubWeaponId, int32 NewTeamNumber)
{
	UE_LOG(LogTemp, Log, TEXT("ASPlayerController::Server_LoadPlayerStateInfo() called, PlayerName : %s"), *NewPlayerName);

	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("SPlayerState is nullptr"));
		return;
	}

	SPlayerState->SetPlayerName(NewPlayerName);
	SPlayerState->SetWeaponId(NewWeaponId);
	SPlayerState->SetSubWeaponId(NewSubWeaponId);
	SPlayerState->SetTeamNumber(NewTeamNumber);

	bLoadPlayerState = true;
	RequestRestartPlayerDelegate.Broadcast(this);
}

bool ASPlayerController::Server_LoadPlayerStateInfo_Validate(const FString& NewPlayerName, int32 NewWeaponId, int32 NewSubWeaponId, int32 NewTeamNumber)
{
	return true;
}