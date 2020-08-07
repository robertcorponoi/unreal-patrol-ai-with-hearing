#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h" 
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"

/**
 * Sets the default values for the PlayerCharacter. In our we case create the necessary 
 * components and set the default values for them.
 */
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach the SpringArmComponent to the player and set its default position
	// and rotation.
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraSpringArm->SetWorldLocationAndRotation(FVector(0.0, 0.0, 70.0), FRotator(0.0, -20.0, 0.0));
	CameraSpringArm->SetupAttachment(GetMesh());

	// Attach the CameraComponent to the SpringArmComponent and set its default
	// position and rotation.
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetWorldLocationAndRotation(FVector(0.0, 0.0, 70.0), FRotator(-20.0, 20.0, 0.0));
	PlayerCamera->SetupAttachment(CameraSpringArm);

	// Create a NoiseEmitterComponent so that we can make noise and have the
	// PatrolCharacter respond to it.
	PlayerNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
	PlayerNoiseEmitter->SetAutoActivate(true);
}

/**
 * Called when the game starts or when spawned
 */
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Called every frame.
 *
 * @param DeltaTime The time between the previous Tick and the current one.
 */
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Called to bind functionality to input.
 *
 * @param PlayerInputComponent An Actor component that enables us to bind axis events and action inputs to methods.
 */
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// When the mouse is moved we turn the player.
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);

	// When the "Move_ForwardBack" or "Move_LeftRight" axis is used we call the methods to make
	// that happen.
	PlayerInputComponent->BindAxis("Move_ForwardBack", this, &APlayerCharacter::MoveForwardBack);
	PlayerInputComponent->BindAxis("Move_LeftRight", this, &APlayerCharacter::MoveLeftRight);

	// When the "Crouch" action is pressed and held down we run the `OnPlayerCrouchStart` method to make
	// the PlayerCharacter crouch and when it's let go, we run `OnPlayerCrouchEnd` to make the Player end
	// the crouch action.
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::OnPlayerCrouchStart);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APlayerCharacter::OnPlayerCrouchEnd);
}

/**
 * When the player moves forwards or backwards we add forward movement
 * multiplied by the axis value. A positive axis value will result in
 * forward movement and vice-versa.
 *
 * @param Value The input axis value.
 */
void APlayerCharacter::MoveForwardBack(float Value)
{
	APlayerCharacter::AddMovementInput(GetActorForwardVector(), Value);
	PlayerNoiseEmitter->MakeNoise(this, 1.0f, GetActorLocation());
}

/**
 * When the player moves left or right we add movement to the right
 * multiplied by the axis value. A positive axis value will result in
 * movement to the right and vice-versa.
 *
 * @param Value The input axis value.
 */
void APlayerCharacter::MoveLeftRight(float Value)
{
	APlayerCharacter::AddMovementInput(GetActorRightVector(), Value);
	//PlayerNoiseEmitter->MakeNoise(this, .5f, GetActorLocation());
}

/**
 * When the "Couch" action is pressed and held down we set `bWantsToCrouch`
 * to true so that the PlayerCharacter will start crouching.
 */
void APlayerCharacter::OnPlayerCrouchStart()
{
	// Only crouch if we can, which in our case should be all the time.
	if (GetCharacterMovement() == nullptr) return;
	if (CanCrouch()) GetCharacterMovement()->bWantsToCrouch = true;

	PlayerNoiseEmitter->MakeNoise(this, 1.0f, GetActorLocation());
}

/**
 * When the "Crouch" action button is released we set `bWantsToCrouch` to
 * false so that the PlayerCharacter will stop crouching.
 */
void APlayerCharacter::OnPlayerCrouchEnd()
{
	if (GetCharacterMovement() == nullptr) return;
	GetCharacterMovement()->bWantsToCrouch = false;
}