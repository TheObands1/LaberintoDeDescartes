// Fill out your copyright notice in the Description page of Project Settings.


#include "LDD_NPC.h"
#include "PaperFlipbookComponent.h"

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

	BP_BeginDialogue();
}

void ALDD_NPC::EndDialogue()
{
	BP_EndDialogue();
}
