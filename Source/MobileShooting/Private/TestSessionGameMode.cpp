// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSessionGameMode.h"

ATestSessionGameMode::ATestSessionGameMode()
{

}

void ATestSessionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//UE_LOG(LogTemp, Log, TEXT("Player %s Login"), *NewPlayer->GetName());

	PlayerControllerList.Add(NewPlayer);

	for (auto SPlayerController : PlayerControllerList)
	{
		UE_LOG(LogTemp, Log, TEXT("Player %s Login"), *SPlayerController->GetName());
	}

	// TODO : ÆÀ ¼³Á¤ Ã³¸®
}