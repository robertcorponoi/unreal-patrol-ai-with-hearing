#include "Waypoint.h"
#include "PatrolCharacter.h"
#include "Components/BoxComponent.h"

/**
 * Sets the default values for the Waypoint. In our case it sets up the BoxComponent
 * and sets it to be a collision trigger.
 */
AWaypoint::AWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the BoxComponent, set it to be a collision trigger, define the method
	// to call when an actor enters the collision zone, and add it to the RootComponent.
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWaypoint::OnWaypointBeginOverlap);
	BoxComponent->SetupAttachment(RootComponent);
}

/**
 * Called when the game starts or when spawned
 */
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * Called every frame.
 *
 * @param DeltaTime The time between the previous Tick and the current one.
 */
void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * When a component first enters the BoxCollider's collision area we try to
 * cast it to our PatrolCharacter and then set a timer so that the PatrolCharacter
 * can run its looking animation. Lastly we set the PatrolCharacter to move on to
 * the next Waypoint.
 *
 * @param OverlappedComp
 * @param OtherActor
 * @param OtherComp
 * @param OtherBodyIndex
 * @param bFromSweep
 * @param SweepResult
 */
void AWaypoint::OnWaypointBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Return if anything is null so we can avoid a potential crash.
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;

	// Try to cast the `OtherActor` to our PatrolCharacter because we only
	// want to work with the PatrolCharacter.
	APatrolCharacter* PatrolCharacter = Cast<APatrolCharacter>(OtherActor);
	if (PatrolCharacter == nullptr) return;

	// Set the PatrolCharacter to be at the Waypoint so that it can play its looking around animation and then
	// call the `AfterWaypointTimer` method when the timer is finished.
	PatrolCharacter->bIsAtWaypoint = true;
}
