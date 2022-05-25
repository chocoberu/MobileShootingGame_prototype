// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamNormalGameMode.h"
#include "SPlayerController.h"
#include "SPlayerState.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ATeamNormalGameMode::ATeamNormalGameMode()
{

}

void ATeamNormalGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("Player %s Login"), *NewPlayer->GetName());
}

void ATeamNormalGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ATeamNormalGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("Restart Player : %s"), *NewPlayer->GetName());
}

AActor* ATeamNormalGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// TODO : �ش� �κ� ���� �ʿ�

	ASPlayerController* PlayerController = Cast<ASPlayerController>(Player);
	if (nullptr == PlayerController)
	{
		UE_LOG(LogTemp, Log, TEXT("Player %s is not SPlayerController"), *Player->GetName());
	}
	ASPlayerState* PlayerState = PlayerController->GetPlayerState<ASPlayerState>();
	if (nullptr == PlayerState)
	{
		UE_LOG(LogTemp, Log, TEXT("Player %s's PlayerState is nullptr"), *Player->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerName : %s"), *PlayerState->GetPlayerName());
	}

	// PlayerStart �߿��� PlayerStartTag�� ��ġ�ϴ� ���� ����
	TArray<AActor*> Results;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Results);

	for (auto Iter : Results)
	{
		APlayerStart* Start = Cast<APlayerStart>(Iter);
		if (nullptr == Iter)
		{
			continue;
		}
		UE_LOG(LogTemp, Log, TEXT("Player Start : %s"), *Start->GetName());

		if (nullptr != PlayerState && PlayerState->GetPlayerName().Compare(Start->PlayerStartTag.ToString()) == 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Find Player Start"));
			Start->PlayerStartTag = TEXT("Taken");
			return Iter;
		}
	}

	// �ش� PlayerStart�� ���� ��� ���� ������� ����
	return Super::ChoosePlayerStart_Implementation(Player);
}
