// Copyright Epic Games, Inc. All Rights Reserved.

#include "PalaceManagerGameMode.h"
#include "PalaceManagerCharacter.h"
#include "UObject/ConstructorHelpers.h"

APalaceManagerGameMode::APalaceManagerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
