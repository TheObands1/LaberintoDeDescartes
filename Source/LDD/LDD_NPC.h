// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "LDD_NPC.generated.h"

/**
 * 
 */

class UPaperFlipbook;
class UAudioComponent;
class USoundCue;
class ALDD_LevelMusic;

UCLASS()
class LDD_API ALDD_NPC : public APaperCharacter
{
	GENERATED_BODY()

protected:

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundCue* EmotionMusic;
	
	ALDD_LevelMusic* LevelMusicRef;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	bool bHasPlayerAlreadyInteractedWithIt;

public:
	ALDD_NPC();

	void BeginDialogue();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_BeginDialogue();

	void EndDialogue();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_EndDialogue();
	
};
