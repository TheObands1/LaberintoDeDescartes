// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LDD_LevelMusic.generated.h"

class UAudioComponent;
class UBillboardComponent;
class ALDD_GameMode;
class ALDD_Character;
class ALDD_NPC;

UCLASS()
class LDD_API ALDD_LevelMusic : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBillboardComponent* MusicBillboardComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* AmbianceAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* MusicAudioComponent;
	
public:	
	// Sets default values for this actor's properties
	ALDD_LevelMusic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	UFUNCTION()
	void StopLevelMusic();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
