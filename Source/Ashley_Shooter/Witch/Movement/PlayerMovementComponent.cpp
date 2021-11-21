// Fill out your copyright notice in the Description page of Project Settings.


#include "./PlayerMovementComponent.h"
#include "GameFramework/Character.h"

UPlayerMovementComponent::UPlayerMovementComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SprintSpeedMultiplier = 2.0f;
    SprintAccelerationMultiplier = 2.0f;
    bAllowMultiDirectionalSprinting = true;
    SprintDirectionRestriction = 0.7f;
}

//============================================================================================
//Replication
//============================================================================================

//Set input flags on character from saved inputs
void UPlayerMovementComponent::UpdateFromCompressedFlags(uint8 Flags)//Client only
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	bWantsToSprint = (Flags&FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UPlayerMovementComponent::SetSprinting(bool bSprinting)
{
	bWantsToSprint = bSprinting;
}

float UPlayerMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (bWantsToSprint && IsMovingForward())
	{
		MaxSpeed *= SprintSpeedMultiplier;
	}

	return MaxSpeed;
}

float UPlayerMovementComponent::GetMaxAcceleration() const
{
	float MaxAccel = Super::GetMaxAcceleration();

	if (bWantsToSprint && IsMovingForward())
	{
		MaxAccel *= SprintAccelerationMultiplier;
	}

	return MaxAccel;
}

bool UPlayerMovementComponent::IsMovingForward() const
{
	if (!PawnOwner)
	{
		return false;
	}

    if (bAllowMultiDirectionalSprinting) {
        return true;
    }

	FVector Forward = PawnOwner->GetActorForwardVector();
	FVector MoveDirection = Velocity.GetSafeNormal();

	//Ignore vertical movement
	Forward.Z = 0.0f;
	MoveDirection.Z = 0.0f;

	float VelocityDot = FVector::DotProduct(Forward, MoveDirection);
	return VelocityDot > SprintDirectionRestriction;    //Check to make sure difference between headings is not too great.
}

class FNetworkPredictionData_Client* UPlayerMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	check(PawnOwner->GetLocalRole() < ROLE_Authority);

	if (!ClientPredictionData)
	{
		UPlayerMovementComponent* MutableThis = const_cast<UPlayerMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_MyMovement(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void FSavedMove_MyMovement::Clear()
{
	Super::Clear();

	//Clear variables back to their default values.
	bSavedWantsToSprint = false;
}

//This is where we compress the flags saved in SetMoveFor. We're basically just ORing a bunch of them together.
uint8 FSavedMove_MyMovement::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

bool FSavedMove_MyMovement::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
    //This pretty much just tells the engine if it can optimize by combining saved moves. There doesn't appear to be
	//any problem with leaving it out, but it seems that it's good practice to implement this anyways.
	if (bSavedWantsToSprint != ((FSavedMove_MyMovement*)&NewMove)->bSavedWantsToSprint)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void FSavedMove_MyMovement::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UPlayerMovementComponent* CharMov = Cast<UPlayerMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{
		//This is literally just the exact opposite of UpdateFromCompressed flags. We're taking the input
		//from the player and storing it in the saved move.
		bSavedWantsToSprint = CharMov->bWantsToSprint;
	}
}

void FSavedMove_MyMovement::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UPlayerMovementComponent* CharMov = Cast<UPlayerMovementComponent>(Character->GetCharacterMovement());
	if (CharMov)
	{
		
	}
}

FNetworkPredictionData_Client_MyMovement::FNetworkPredictionData_Client_MyMovement(const UCharacterMovementComponent& ClientMovement)
: Super(ClientMovement)
{

}

FSavedMovePtr FNetworkPredictionData_Client_MyMovement::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_MyMovement());
}