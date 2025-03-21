// Copyright Epic Games, Inc. All Rights Reserved.

#include "PersonalProj2_GP4GameMode.h"
#include "PersonalProj2_GP4Character.h"
#include "UObject/ConstructorHelpers.h"

APersonalProj2_GP4GameMode::APersonalProj2_GP4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
