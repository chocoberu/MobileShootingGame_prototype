// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileShootingGameMode.h"
#include "MobileShootingCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "SPlayerController.h"

AMobileShootingGameMode::AMobileShootingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;		
	}

	PlayerControllerClass = ASPlayerController::StaticClass();
}
