#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "PatrolAIController.generated.h"

/**
 * Configures the senses of the PatrolCharacter and responds to sensing the PlayerCharacter
 * set as the target in the PatrolCharacter instance.
 */
UCLASS()
class PATROLAITUTORIAL_API APatrolAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this AI Controller's properties.
	APatrolAIController();

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

	// Called when the AIController is taken over.
	virtual void OnPossess(APawn* Pawn) override;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	// Called to get the control's rotation.
	virtual FRotator GetControlRotation() const override;

	// Called when a move request has been completed. This can be
	// a move request to a Waypoint or to the PlayerCharacter.
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	// Gets called when the perception component updates. This is
	// where check to see if the PlayerCharacter was detected.
	UFUNCTION()
	void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	// Moves the PatrolCharacter to the next Waypoint.
	UFUNCTION()
	void MoveToWaypoint();

	// The TimerHandle used to delay the setting of the waypoint giving the
	// PatrolCharacter time to perform a looking animation.
	UPROPERTY()
	FTimerHandle TimerHandle;

	// The position to move to. This can be different than the PlayerCharacter's
	// position if a distraction was used.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector PositionToMoveTo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class APatrolCharacter* PatrolCharacter;

	// A reference to the hearing perception component.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	class UAISenseConfig_Hearing* HearingConfig;

	// The range at which the PatrolCharacter can hear the PlayerCharacter.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AIHearingRange = 500.0f;

	// The amount of time after the PlayerCharacter has been heard that the
	// PatrolCharacter will forget they heard the PlayerCharacter.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float AIHearingMaxAge = 5.0f;

	// Indicates whether the PatrolCharacter will be able to listen at the
	// start of the game or whether it will be enabled later manually.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	bool bAIHearingStartsEnabled = true;

	// How far to the side that the PatrolCharacter can see the PlayerCharacter.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float AIPeripheralVisionAngleDegrees = 90.0f;

	// Indicates whether the PatrolCharacter has seen the PlayerCharacter or not.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	bool bIsPlayerDetected = false;

	// If the PatrolCharacter has seen the PlayerCharacter then this is populated with how
	// far away the PlayerCharacter is from the PatrolCharacter.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	float DistanceToPlayer = 0.0f;

	UPROPERTY()
	float AccruedDelta = 0.0f;
};