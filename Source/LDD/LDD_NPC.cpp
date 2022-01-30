// Fill out your copyright notice in the Description page of Project Settings.


#include "LDD_NPC.h"
#include "PaperFlipbookComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "LDD_LevelMusic.h"

ALDD_NPC::ALDD_NPC()
{
	bHasPlayerAlreadyInteractedWithIt = false;
}

void ALDD_NPC::BeginDialogue()
{
	if (bHasPlayerAlreadyInteractedWithIt)
	{
		return;
	}

	//LevelMusicRef->StopLevelMusic();
	//UGameplayStatics::SpawnSound2D(GetWorld(), EmotionMusic);

	BP_BeginDialogue();
}

void ALDD_NPC::EndDialogue()
{
	UGameplayStatics::SpawnSound2D(GetWorld(), EmotionMusic);
	BP_EndDialogue();
}
