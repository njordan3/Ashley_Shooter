// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Witch.generated.h"

UCLASS(Blueprintable)
class ASHLEY_SHOOTER_API AWitch : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void Turn(float Value);
	UFUNCTION()
		void LookUp(float Value);
	UFUNCTION()
		void StartJump();
	UFUNCTION()
		void StopJump();

	UPROPERTY(VisibleAnywhere, Category="Controls")
		float TurnRate;
	UPROPERTY(VisibleAnywhere, Category="Controls")
		float LookUpRate;

	// Camera
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* CameraSpringArmComp;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* CameraComponent;

	// First-person mesh (arms), visible only to the owning player.
	UPROPERTY(EditAnywhere, Category="Mesh")
		class USkeletalMeshComponent* FPSMesh;

	class AGunBase* Gun;

	UPROPERTY(EditAnywhere, Category="Gun")
		TSubclassOf<AGunBase> GunClass; 
};
