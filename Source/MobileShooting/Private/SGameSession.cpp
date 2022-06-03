// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameSession.h"
#include "SGameInstance.h"

void ASGameSession::UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId)
{
	USGameInstance* SGameInstance = GetGameInstance<USGameInstance>();
	if (GetNetMode() != NM_Standalone && nullptr != SGameInstance &&
		UniqueId.IsValid() &&
		UniqueId->IsValid())
	{
		SGameInstance->UnregisterPlayer(InSessionName, *UniqueId);
	}
}
