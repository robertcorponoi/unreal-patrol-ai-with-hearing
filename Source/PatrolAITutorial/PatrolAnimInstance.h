#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PatrolAnimInstance.generated.h"

/**
 * Handles updating the properties used by the PatrolAnim_BP Blueprint
 * to decide what animations to play.
 */
UCLASS()
class PATROLAITUTORIAL_API UPatrolAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Indicates whether the PatrolCharacter is moving forward or not.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsMovingForward = false;

	// Indicates whether the PatrolCharacter is looking around for the
	// PlayerCharacter or not.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsLookingAround = false;

	// Indicates whether the PatrolCharacter has been alerted to the
	// PlayerCharacter's presence or not.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAlerted = false;

	// Used by the PatrolAnim_BP Blueprint to update the animation properties
	// and decide what animations to play.
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
	void UpdateAnimationProperties();
};
