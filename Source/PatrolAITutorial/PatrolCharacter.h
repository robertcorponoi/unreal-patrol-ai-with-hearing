#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrolCharacter.generated.h"

class AWaypoint;

/**
 * The modes that the PatrolCharacter can be in. 
 * PATROL - The PatrolCharacter is patrolling between Waypoints.
 * CHASE - The PatrolCharacter is chasing the PlayerCharacter.
 */
UENUM(BlueprintType)
enum class PatrolMode : uint8
{
	PATROL UMETA(DisplayName = "PATROL"),
	CHASE UMETA(DisplayName = "CHASE")
};

/**
 * A Character that patrols around a set of Waypoints and keeps
 * an eye out for the PlayerCharacter.
 */
UCLASS()
class PATROLAITUTORIAL_API APatrolCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties.
	APatrolCharacter();

	// We start the game assuming we can't see the player and so we
	// are in Patrol mode.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	PatrolMode Mode = PatrolMode::PATROL;

	// The speed at which the PatrolCharacter walks between Waypoints.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PatrolSpeed = 100.0f;

	// The speed at which the PatrolCharacter chases the PlayerCharacter.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseSpeed = 500.0f;

	// The Waypoints that the should be patrolled.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AWaypoint*> Waypoints;

	// The index of the Waypoint in `Waypoints` to go towards.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 CurrentWaypointIndex = 0;

	// The distance away the PatrolCharacter can be from the Waypoint before 
	// it registers the PatrolCharacter as being arrived at the Waypoint.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceBeforeArrivedAtWaypoint = 5.0f;

	// The amount of time, in seconds, that the PatrolCharacter should wait
	// at a Waypoint before moving on to the next one.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaypointDelay = 5.0f;

	// The distance away the PatrolCharacter can be from the PlayerCharacter
	// before it stops chasing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceBeforeArrivedAtPlayer = 100.0f;

	// Indicates whether this PatrolCharacter is currently at a Waypoint or
	// not. This is used by the animation Blueprint to decide if the Looking
	// or Walking animation should be played.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAtWaypoint = false;

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to change the PatrolCharacter's settings to patrol mode.
	void SetPatrolMode();

	// Called to change the PatrolCharacter's settings to chase mode.
	void SetChaseMode();
};
