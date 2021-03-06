// Copyright Epic Games, Inc. All Rights Reserved.

#include "LDDCharacter.h"
#include "LDD_NPC.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// ALDDCharacter

ALDDCharacter::ALDDCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	CameraBoom->SetUsingAbsoluteRotation(true);
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	//GetCharacterMovement()->bConstrainToPlane = true;
	//GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);


	NPCDetectorCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("NPCDetectorCollider"));
	NPCDetectorCollider->SetupAttachment(RootComponent);

	StepSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StepSoundComponent"));
	StepSoundComponent->SetupAttachment(RootComponent);

	JumpSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("JumpSoundComponent"));
	JumpSoundComponent->SetupAttachment(RootComponent);

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;



}

//////////////////////////////////////////////////////////////////////////
// Animation

void ALDDCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(NPCDetectorCollider))
	{
		NPCDetectorCollider->OnComponentBeginOverlap.AddDynamic(this, &ALDDCharacter::NPCOnBeginOverlap);
		NPCDetectorCollider->OnComponentEndOverlap.AddDynamic(this, &ALDDCharacter::NPCOnEndOverlap);
	}
	
}

void ALDDCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
	UPaperFlipbook* DesiredAnimation;

	// Are we moving or standing still?
	if (PlayerSpeedSqr > 0.0f) 
	{
		DesiredAnimation = RunningAnimation;
		StepSoundComponent->Play();
	}
	else
	{
		DesiredAnimation = IdleAnimation;
	}
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void ALDDCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
}


//////////////////////////////////////////////////////////////////////////
// Input

void ALDDCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALDDCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALDDCharacter::MoveRight);

	PlayerInputComponent->BindAxis("MoveFoward", this, &ALDDCharacter::MoveFoward);

	PlayerInputComponent->BindAxis("RotateCamera", this, &ALDDCharacter::RotateCamera);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALDDCharacter::Interact);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALDDCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALDDCharacter::TouchStopped);
}

void ALDDCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	//AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
	//AddMovementInput(GetActorForwardVector() * Value);
	AddMovementInput(GetActorRightVector() * Value);
}

void ALDDCharacter::MoveFoward(float Value)
{
	//AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
	//AddMovementInput(GetActorRightVector()*Value);
	AddMovementInput(GetActorForwardVector() * Value);
}

void ALDDCharacter::Jump()
{
	Super::Jump();

	JumpSoundComponent->Play();
}

void ALDDCharacter::RotateCamera(float Value)
{
	BP_RotateCamera(Value);
}

void ALDDCharacter::Interact()
{
	if (CurrentNPCReference != nullptr)
	{
		CurrentNPCReference->BeginDialogue();
		BP_Interact();
	}
}

void ALDDCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void ALDDCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void ALDDCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		/*
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
		*/
	}
}

void ALDDCharacter::NPCOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	if (IsValid(OtherActor))
	{
		ALDD_NPC* PossibleNPC = Cast<ALDD_NPC>(OtherActor);
		if (IsValid(PossibleNPC))
		{
			CurrentNPCReference = PossibleNPC;
		}
	}
}

void ALDDCharacter::NPCOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		ALDD_NPC* PossibleNPC = Cast<ALDD_NPC>(OtherActor);
		if (IsValid(PossibleNPC))
		{
			if (CurrentNPCReference != nullptr)
			{
				CurrentNPCReference = nullptr;
			}
		}
	}
}
