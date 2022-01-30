// Copyright Epic Games, Inc. All Rights Reserved.

#include "LDDGameMode.h"
#include "LDDCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LDD_FuryNPC.h"
#include "LDD_FearNPC.h"
#include "LDD_Happiness.h"

ALDDGameMode::ALDDGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ALDDCharacter::StaticClass();	
}

void ALDDGameMode::BeginPlay()
{
	Super::BeginPlay();

	/*FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnVector;
	SpawnVector.X = FMath::FRandRange(0, 100);
	SpawnVector.Y = FMath::FRandRange(0, 100);
	SpawnVector.Z = 0;
	UE_LOG(LogTemp, Warning, TEXT("SPAWN VECTOR: %s"), *SpawnVector.ToString());
	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY"));

	GetWorld()->SpawnActor<ALDD_FuryNPC>(FearClass, SpawnVector, FRotator::ZeroRotator, SpawnParameters);*/
}
