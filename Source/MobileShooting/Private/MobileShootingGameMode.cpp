// Copyright Epic Games, Inc. All Rights Reserved.

#include "MobileShootingGameMode.h"
#include "MobileShootingCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "SPlayerController.h"
#include "SCharacter.h"

AMobileShootingGameMode::AMobileShootingGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;		
	}*/
	DefaultPawnClass = ASCharacter::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
}
