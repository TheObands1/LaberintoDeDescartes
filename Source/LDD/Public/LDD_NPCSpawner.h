// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LDD_NPCSpawner.generated.h"

class ALDD_NPC;
class ALDD_FuryNPC;
class ALDD_SadnessNPC;
class ALDD_Happiness;

UCLASS()
class LDD_API ALDD_NPCSpawner : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MakeEditWidget = true))
	TArray<FVector> SpawnPoints;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPCs")
	TSubclassOf<ALDD_FuryNPC> FuryClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPCs")
	TSubclassOf<ALDD_SadnessNPC> SadnessClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPCs")
	TSubclassOf<ALDD_Happiness> HappinessClass;

	TArray<TSubclassOf<ALDD_NPC>> NPCArray;

public:	
	// Sets default values for this actor's properties
	ALDD_NPCSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetSpawnPoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
