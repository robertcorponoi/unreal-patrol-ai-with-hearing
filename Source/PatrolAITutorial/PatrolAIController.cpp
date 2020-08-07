#include "PatrolAIController.h"
#include "Waypoint.h"
#include "PatrolCharacter.h"
#include "PlayerCharacter.h"
#include "Tasks/AITask_MoveTo.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "Navigation/PathFollowingComponent.h"

/**
 * Sets the default values for the PatrolAIController. In our case we create the necessary
 * perception components and setup the dominant sense.
 */
APatrolAIController::APatrolAIController()
{
	// We need this AIController to run its Tick method.
	PrimaryActorTick.bCanEverTick = true;

	// Create the hearing sense, and a AIPerceptionComponent and 
	// set it as the default perception component of the AIController.
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	HearingConfig->HearingRange = AIHearingRange;
	HearingConfig->LoSHearingRange = AIHearingRange;
	HearingConfig->SetMaxAge(AIHearingMaxAge);

	// Set the hearing sense to detect everything. This should be changed to fit your needs
	// but it makes it easiest for the tutorial.
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Set hearing as the dominant sense.
	GetPerceptionComponent()->SetDominantSense(*HearingConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*HearingConfig);
}

/**
 * Called when the game starts or when spawned
 */
void APatrolAIController::BeginPlay()
{
	Super::BeginPlay();

	// Set the method to respond when the perception component updates.
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &APatrolAIController::OnPawnDetected);

	// Set the reference to the PatrolCharacter.
	PatrolCharacter = Cast<APatrolCharacter>(GetPawn());

	// Set the Waypoint index to the last Waypoint because the `MoveToWaypoint` method
	// of the PatrolAIController will increment by 1 on the first update so by setting
	// it to the last index we end up at the first.
	PatrolCharacter->CurrentWaypointIndex = PatrolCharacter->Waypoints.Num() - 1;

	// Tell the PatrolCharacter to start moving to a Waypoint.
	MoveToWaypoint();
}

/**
 * Called when the AIController is taken over.
 *
 * @param PatrolPawn The pawn that was taken over.
 */
void APatrolAIController::OnPossess(APawn* PatrolPawn)
{
	Super::OnPossess(PatrolPawn);
}

/**
 * Called every frame.
 *
 * @param DeltaTime The time between the previous Tick and the current one.
 */
void APatrolAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

/**
 * Returns a zero rotator if GetPawn is null and the Actor rotation yaw
 * otherwise.
 */
FRotator APatrolAIController::GetControlRotation() const
{
	if (GetPawn() == nullptr) return FRotator(0.0f, 0.0f, 0.0f);

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

/**
 * When the PatrolCharacter's perception detects a Pawn we check to see if
 * it is the PlayerCharacter and update the `bIsPlayerDetected` variable accordingly.
 *
 * @param DetectedPawns - The Pawns that the AIController has detected in its radius.
 */
void APatrolAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	// Since `DetectedPawns` is an array of pawns that have been detected we
	// need to loop through and see if any of those are our PlayerController.
	for (AActor* DetectedPawn : DetectedPawns)
	{
		// Get the data from the latest hearing sensed.
		if (!HearingConfig->GetSenseID().IsValid()) return;
		const FActorPerceptionInfo* HeardPerceptionInfo = GetPerceptionComponent()->GetFreshestTrace(HearingConfig->GetSenseID());

		// If the sense isn't active anymore then we return early. This happens when
		// the max age is hit for a heard event.
		if (!HeardPerceptionInfo->IsSenseActive(HearingConfig->GetSenseID()))
		{
			// The hearing sense has been lost so we return the PatrolCharacter back
			// to patrolling Waypoints.
			MoveToWaypoint();
			return;
		}

		// Attempt to cast the current Pawn to our PlayerCharacter and return early
		// if it's not since we only care about the PlayerCharacter.
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(DetectedPawn);
		if (PlayerCharacter == nullptr) return;
		
		// At this point we can safely assume that the PlayerCharacter has been detected
		// and we can get the distance away they are from the PatrolCharacter.
		bIsPlayerDetected = true;
		DistanceToPlayer = GetPawn()->GetSquaredDistanceTo(PlayerCharacter);

		// Instead of just moving towards the PlayerCharacter we want to get the position that
		// the hearing event happened at. This is helpful if you're implementing a distraction
		// hearing event and don't want to create another Actor just for a distraction.
		PositionToMoveTo = HeardPerceptionInfo->GetStimulusLocation(HearingConfig->GetSenseID());

		PatrolCharacter->SetChaseMode();

		// Finally we move to the location found above. If the location keeps moving, like if
		// the sound is the PlayerCharacter's footsteps, then this will keep moving to the new
		// location.
		MoveToLocation(PositionToMoveTo, PatrolCharacter->DistanceBeforeArrivedAtPlayer);

		// DEBUG
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, FString::Printf(TEXT("Investigating Sound At Location: %s For %f seconds"), *PositionToMoveTo.ToString(), AIHearingMaxAge));
		}
	}
}

/**
 * When a `MoveToLocation` or `MoveToActor` action has completed this method is fired
 * and we either move to another Waypoint or we chase the PlayerCharacter.
 *
 * @param RequestID The Move Request ID for the move that was completed.
 * @param Result
 */
void APatrolAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	if (PatrolCharacter->Mode == PatrolMode::PATROL)
	{
		// DEBUG
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, FString::Printf(TEXT("Patrol At Waypoint: %s For %f seconds"), *GetNameSafe(PatrolCharacter->Waypoints[PatrolCharacter->CurrentWaypointIndex]), PatrolCharacter->WaypointDelay));
		}

		// The PatrolCharacter is in Patrol mode so we wait at the current Waypoint
		// for 5 seconds and then we call `MoveToWaypoint` to move to the next Waypoint.
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APatrolAIController::MoveToWaypoint, PatrolCharacter->WaypointDelay);
	}
}

/**
 * Moves the PatrolCharacter to the next Waypoint on its list.
 */
void APatrolAIController::MoveToWaypoint()
{
	// Check to see if the PatrolCharacter was in a chase previous to this being
	// called. If they were then change back to patrol mode and we don't increment
	// the Waypoint index because chances are they didn't make it to the Waypoint
	// before being interrupted so we don't want them to skip a Waypoint.
	//
	// If they were not in a chase then we increment the Waypoint index so they can
	// go to the next Waypoint.
	if (PatrolCharacter->Mode == PatrolMode::CHASE) 
	{
		// Since the PatrolCharacter was previously chasing the PlayerCharacter we
		// can assume that they didn't make it to their Waypoint. To compensate for
		// this we set the Waypoint index back by 1.
		PatrolCharacter->SetPatrolMode();
	}
	else
	{
		if (PatrolCharacter->CurrentWaypointIndex == PatrolCharacter->Waypoints.Num() - 1)
		{
			// The PatrolCharacter is at the last Waypoint in the Waypoints array so we send
			// them back to the first Waypoint.
			PatrolCharacter->CurrentWaypointIndex = 0;
		}
		else
		{
			// There's still more Waypoints the PatrolCharacter hasn't patrolled yet so we
			// send the PatrolCharacter to the next Waypoint in the Waypoints array.
			PatrolCharacter->CurrentWaypointIndex++;
		}
	}

	// The Waypoint exists, the PatrolCharacter is not currently waiting at a Waypoint,
	// and the PlayerCharacter has not been detected so we move towards a Waypoint.
	MoveToActor(PatrolCharacter->Waypoints[PatrolCharacter->CurrentWaypointIndex], PatrolCharacter->DistanceBeforeArrivedAtWaypoint);

	// DEBUG
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, FString::Printf(TEXT("Patrol moving To Waypoint: %s"), *GetNameSafe(PatrolCharacter->Waypoints[PatrolCharacter->CurrentWaypointIndex]), PatrolCharacter->CurrentWaypointIndex));
	}

	PatrolCharacter->bIsAtWaypoint = false;
}