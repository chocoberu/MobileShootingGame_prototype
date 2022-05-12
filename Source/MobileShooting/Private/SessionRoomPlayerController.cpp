// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionRoomPlayerController.h"
#include "SPlayerState.h"
#include "TestSessionGameMode.h"
#include "SGameInstance.h"
#include "TestSaveGame.h"
#include "UI/TestSessionRoomWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASessionRoomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bIsSessionHost = false;

	if (false == IsLocalPlayerController() || nullptr == SessionRoomUIClass)
	{
		return;
	}

	SessionRoomWidget = CreateWidget<UTestSessionRoomWidget>(this, SessionRoomUIClass);
	if (nullptr == SessionRoomWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("SessionRoom Widget is nullptr"));
		return;
	}

	if (GetLocalRole() != ROLE_Authority)
	{
		SessionRoomWidget->SetStartButtonVisible(false);
	}
	else
	{
		bIsSessionHost = true;
	}

	SessionRoomWidget->AddToViewport();

	RequestServerPlayerList();
}

void ASessionRoomPlayerController::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	UE_LOG(LogTemp, Log, TEXT("ASessionRoomPlayerController::EndPlay() called, PlayerController : %s"), *GetName());	
}

void ASessionRoomPlayerController::UpdatePlayerList(const TArray<FRoomPlayerInfo>& PlayerInfoList)
{
	int32 Index = 0;
	for (const FRoomPlayerInfo& PlayerInfo : PlayerInfoList)
	{
		FString PlayerName;
		if (0 == PlayerInfo.PlayerName.Compare(GetPlayerName()))
		{
			PlayerName = TEXT("ME");
		}
		else
		{
			PlayerName = PlayerInfo.PlayerName;
		}

		SessionRoomWidget->SetPlayerRowByIndex(Index, PlayerName, PlayerInfo.bPlayerReady);
		Index++;
	}

	// 비어있는 Row 초기화
	// 하드 코딩 수정 필요
	for (; Index < 4; Index++)
	{
		SessionRoomWidget->SetPlayerRowByIndex(Index, TEXT("Empty"), false);
	}
}

void ASessionRoomPlayerController::RequestServerPlayerList()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ATestSessionGameMode* SessionGameMode = Cast<ATestSessionGameMode>(GetWorld()->GetAuthGameMode());
		if (nullptr != SessionGameMode)
		{
			SessionGameMode->UpdatePlayerList();
		}
	}
	else
	{
		// 서버 측에 요청 필요
		Server_RequestServerPlayerList();
	}
}

void ASessionRoomPlayerController::Server_RequestServerPlayerList_Implementation()
{
	RequestServerPlayerList();
}

bool ASessionRoomPlayerController::Server_RequestServerPlayerList_Validate()
{
	return true;
}

void ASessionRoomPlayerController::ChangeReadyState()
{
	UE_LOG(LogTemp, Log, TEXT("Change Ready State"));
	
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return;
	}

	bool bReady = IsPlayerReady();

	if (false == bReady)
	{
		UTestSaveGame* NewSaveGame = NewObject<UTestSaveGame>();
		NewSaveGame->MainWeaponId = SPlayerState->GetWeaponId();
		NewSaveGame->SubWeaponId = SPlayerState->GetSubWeaponId();
		NewSaveGame->TeamNum = SPlayerState->GetTeamNumber();

		if (true == UGameplayStatics::SaveGameToSlot(NewSaveGame, SPlayerState->GetPlayerName(), 0))
		{
			UE_LOG(LogTemp, Log, TEXT("Save Success"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save data!"));
			return;
		}
	}

	ReadyGame(!bReady);
}

void ASessionRoomPlayerController::ReadyGame(bool bReadyState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		// PlayerState에 값을 저장
		ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
		if (nullptr != SPlayerState)
		{
			SPlayerState->SetPlayerReadyState(bReadyState);
		}

		// 이후 플레이어 리스트 업데이트
		RequestServerPlayerList();
		Client_ReadyGame(bReadyState);
	}
	else
	{
		// 서버 측에 요청 필요
		Server_ReadyGame(bReadyState);
	}
}

void ASessionRoomPlayerController::Server_ReadyGame_Implementation(bool bReadyState)
{
	ReadyGame(bReadyState);
}

bool ASessionRoomPlayerController::Server_ReadyGame_Validate(bool bReadyState)
{
	return true;
}

void ASessionRoomPlayerController::Client_ReadyGame_Implementation(bool bReadyState)
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return;
	}
	SPlayerState->SetPlayerReadyState(bReadyState);

	// bReadyState == true인 경우 SelectWeaponButton을 비활성화
	SessionRoomWidget->SetEnableSelectWeaponButton(!bReadyState);
	SessionRoomWidget->SetEnablePrevButton(!bReadyState);
}

void ASessionRoomPlayerController::StartGame()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ATestSessionGameMode* SessionGameMode = Cast<ATestSessionGameMode>(GetWorld()->GetAuthGameMode());
		if (nullptr == SessionGameMode)
		{
			return;
		}
		if (false == SessionGameMode->StartGame())
		{
			USGameInstance* SGameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance());
			if (nullptr != SGameInstance)
			{
				// TEST CODE
				SGameInstance->ShowErrorMessage(0);
			}
		}
	}
}

void ASessionRoomPlayerController::Client_UpdatePlayerList_Implementation(const TArray<FRoomPlayerInfo>& PlayerInfoList)
{
	UpdatePlayerList(PlayerInfoList);
}

void ASessionRoomPlayerController::LeaveSession(bool bSessionHost)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ATestSessionGameMode* SessionGameMode = Cast<ATestSessionGameMode>(GetWorld()->GetAuthGameMode());
		if (nullptr != SessionGameMode)
		{
			// TODO : SessionHost가 나갈 때 다른 클라이언트 kick 처리
			SessionGameMode->LeaveSession(GetPlayerName(), bSessionHost);
		}
	}
	else
	{
		Server_LeaveSession();
	}
}

void ASessionRoomPlayerController::Server_LeaveSession_Implementation()
{
	LeaveSession(false);
}

bool ASessionRoomPlayerController::Server_LeaveSession_Validate()
{
	return true;
}

void ASessionRoomPlayerController::Client_LeaveSession_Implementation(bool bKicked)
{
	// TODO : bKicked인 경우 ErrorMessage 처리 필요
	USGameInstance* SGameInstance = Cast<USGameInstance>(GetWorld()->GetGameInstance());
	if (nullptr != SGameInstance)
	{
		if (true == bKicked)
		{
			SGameInstance->ShowErrorMessage(0);
		}
		SGameInstance->LeaveAndDestroySession();
	}
	
}

void ASessionRoomPlayerController::SetPlayerName(const FString NewName)
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return;
	}

	SPlayerState->SetPlayerName(NewName);
}

FString ASessionRoomPlayerController::GetPlayerName()
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return FString();
	}

	return SPlayerState->GetPlayerName();
}

bool ASessionRoomPlayerController::IsPlayerReady() const
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return false;
	}

	return SPlayerState->IsPlayerReady();
}

bool ASessionRoomPlayerController::IsSessionHost() const
{
	return bIsSessionHost;
}

void ASessionRoomPlayerController::SetWeaponId(const int32 WeaponId)
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return;
	}
	SPlayerState->SetWeaponId(WeaponId);
}

void ASessionRoomPlayerController::SetSubWeaponId(const int32 SubWeaponId)
{
	ASPlayerState* SPlayerState = GetPlayerState<ASPlayerState>();
	if (nullptr == SPlayerState)
	{
		return;
	}
	SPlayerState->SetSubWeaponId(SubWeaponId);
}