// Fill out your copyright notice in the Description page of Project Settings.


#include "LDD_NPCSpawner.h"
#include "LDD_FuryNPC.h"
#include "LDD_SadnessNPC.h"
#include "LDD_Happiness.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALDD_NPCSpawner::ALDD_NPCSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALDD_NPCSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(FuryClass) && IsValid(SadnessClass) && IsValid(HappinessClass)) {
		NPCArray.Add(FuryClass);
		NPCArray.Add(SadnessClass);
		NPCArray.Add(HappinessClass);
	}

	if (NPCArray.Num() > 0) {
		for (TSubclassOf<ALDD_NPC> NPC : NPCArray) {
			const FVector LocalSpawnPoint = GetSpawnPoint();
			const FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), LocalSpawnPoint);

			FActorSpawnParameters SpawnParameter;
			SpawnParameter.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//For Spawning actor and, after the game starts, set the BotSpawner that created it:
			GetWorld()->SpawnActor<ALDD_NPC>(NPC, SpawnPoint, FRotator::ZeroRotator, SpawnParameter);
		}
	}
}

FVector ALDD_NPCSpawner::GetSpawnPoint()
{
	if (SpawnPoints.Num() > 0)
	{
		const int IndexSelected = FMath::RandRange(0, (SpawnPoints.Num() - 1));
		FVector CurrentSpawnPoint = SpawnPoints[IndexSelected];

		SpawnPoints.RemoveAt(IndexSelected);

		return CurrentSpawnPoint;
	}
	else
	{
		return GetActorLocation();

	}
}

// Called every frame
void ALDD_NPCSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

