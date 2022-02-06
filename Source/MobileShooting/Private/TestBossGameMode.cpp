// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBossGameMode.h"
#include "SPlayerController.h"
#include "SCharacter.h"

ATestBossGameMode::ATestBossGameMode()
{
	DefaultPawnClass = ASCharacter::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
}