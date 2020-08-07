#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

/**
 * The PlayerCharacter is the default pawn on the game.
 */
UCLASS()
class PATROLAITUTORIAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties.
	APlayerCharacter();

	// The SkeletalMeshComponent of the player which is populated by a skeletal
	// mesh in the Blueprint.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* PlayerSkeletalMesh;

	// The SpringArmComponent that the Camera gets attached to.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* CameraSpringArm;

	// The CameraComponent.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* PlayerCamera;

	// The NoiseEmitterComponent.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPawnNoiseEmitterComponent* PlayerNoiseEmitter;

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Gets called when the "Move_ForwardBack" axis input is used.
	void MoveForwardBack(float Value);

	// Gets called when the "Move_LeftRight" axis input is used.
	void MoveLeftRight(float Value);

	// Gets called when the "Crouch" action is pressed down.
	void OnPlayerCrouchStart();

	// Gets called when the "Crouch" action is released.
	void OnPlayerCrouchEnd();
};
