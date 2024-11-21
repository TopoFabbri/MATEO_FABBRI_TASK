// Copyright Epic Games, Inc. All Rights Reserved.

#include "MATEO_FABBRI_TASKGameMode.h"
#include "MATEO_FABBRI_TASKCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMATEO_FABBRI_TASKGameMode::AMATEO_FABBRI_TASKGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
