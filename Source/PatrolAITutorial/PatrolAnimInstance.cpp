#include "PatrolAnimInstance.h"
#include "Math/Rotator.h"
#include "PatrolCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/**
 * Updates the variables used by the PatrolAnim_BP Blueprint to
 * decide what animations to play.
 */
void UPatrolAnimInstance::UpdateAnimationProperties()
{
	// Try to get the Pawn being animated and return if null.
	APawn* PatrolPawn = TryGetPawnOwner();
	if (!PatrolPawn) return;

	// Try to cast the Pawn to our PatrolCharacter since that's the only
	// thing we want to animate.
	APatrolCharacter* PatrolCharacter = Cast<APatrolCharacter>(PatrolPawn);
	if (PatrolCharacter == nullptr) return;

	// Get the PatrolCharacter's normalized velocity.
	UCharacterMovementComponent* PatrolCharacterMovement = PatrolCharacter->GetCharacterMovement();
	FVector PatrolCharacterMovementVelocity = PatrolCharacterMovement->Velocity;
	PatrolCharacterMovementVelocity.Normalize();

	// Get the PatrolCharacter's CapsuleComponent rotation and un-rotate it.
	UCapsuleComponent* PatrolCapsule = PatrolCharacter->GetCapsuleComponent();
	FRotator PatrolCapsuleRotation = PatrolCapsule->GetComponentRotation();
	FVector PatrolVelocityUnrotated = PatrolCapsuleRotation.UnrotateVector(PatrolCharacterMovementVelocity);

	// Set the moving forward variable based on the unrotated velocity.
	bIsMovingForward = PatrolVelocityUnrotated.X > 0.1;

	// The LookingAround animation is played when the PatrolCharacter is at
	// a Waypoint so we can get this from the PatrolCharacter instance.
	bIsLookingAround = PatrolCharacter->bIsAtWaypoint;
}
