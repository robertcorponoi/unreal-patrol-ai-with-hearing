#include "PatrolCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

/**
 * Sets the default values for the PatrolCharacter. In our case we don't do much
 * here except set up rotation and orientation so that the PatrolCharacter faces
 * the direction they're walking.
 */
APatrolCharacter::APatrolCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Helps orient the PatrolCharacter so that when it walks it doesn't face the
	// Waypoint but instead the direction that it's walking.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Set the default rotation rate and the max walking speed.
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	SetPatrolMode();
}

/**
 * Called when the game starts or when spawned
 */
void APatrolCharacter::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Called every frame.
 *
 * @param DeltaTime The time between the previous Tick and the current one.
 */
void APatrolCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * Called to bind functionality to input.
 *
 * @param PlayerInputComponent An Actor component that enables us to bind axis events and action inputs to methods.
 */
void APatrolCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

/**
 * Used by the `PatrolAIController` to set the properties of the PatrolCharacter to
 * the values they should be when the PatrolCharacter is patrolling Waypoints.
 */
void APatrolCharacter::SetPatrolMode()
{
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	Mode = PatrolMode::PATROL;
}

/**
 * Used by the `PatrolAIController` to set the properties of the PatrolCharacter to
 * the values they should be when the PatrolCharacter is chasing the PlayerCharacter.
 */
void APatrolCharacter::SetChaseMode()
{
	// Set this to false so that the looking animation doesn't play while chasing
	// the PlayerCharacter.
	bIsAtWaypoint = false;

	if (GetCharacterMovement() == nullptr) return;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
	Mode = PatrolMode::CHASE;
}