#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

class APatrolCharacter;

/**
 * Waypoints define the areas that PatrolCharacters patrol and they also update
 * the next waypoint for the PatrolCharacter to go to once the PatrolCharacter
 * has reached a waypoint.
 */
UCLASS()
class PATROLAITUTORIAL_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties.
	AWaypoint();

	// The BoxComponent of this Waypoint which is used as a collision trigger to
	// know when the PatrolCharacter has entered it.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComponent;

	// Called when another component triggers this Waypoints collision and it
	// lets us handle the PatrolCharacter arriving to this Waypoint.
	UFUNCTION()
	void OnWaypointBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;
};
