// Fill out your copyright notice in the Description page of Project Settings.


#include "LDD_LevelMusic.h"
#include "Components/AudioComponent.h"
#include "Components/BillboardComponent.h"
#include "LDD/LDDGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALDD_LevelMusic::ALDD_LevelMusic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MusicBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("MusicBillboardComponent"));
	RootComponent = MusicBillboardComponent;

	AmbianceAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AmbianceAudioComponent"));
	AmbianceAudioComponent->SetupAttachment(RootComponent);

	MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicAudioComponent"));
	MusicAudioComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ALDD_LevelMusic::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALDD_LevelMusic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

