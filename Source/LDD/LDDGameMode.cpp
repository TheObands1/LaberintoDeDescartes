// Copyright Epic Games, Inc. All Rights Reserved.

#include "LDDGameMode.h"
#include "LDDCharacter.h"

ALDDGameMode::ALDDGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ALDDCharacter::StaticClass();	
}
