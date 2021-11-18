// Copyright Epic Games, Inc. All Rights Reserved.


#include "Ashley_ShooterGameModeBase.h"

AAshley_ShooterGameModeBase::AAshley_ShooterGameModeBase()
{
	// Set default pawn class to the Witch BP
	static ConstructorHelpers::FClassFinder<APawn> WitchClassFinder(TEXT("/Game/Witch/BP_Witch"));
	DefaultPawnClass = WitchClassFinder.Class;
}

void AAshley_ShooterGameModeBase::StartPlay()
{
    Super::StartPlay();

    check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Base Gamemode"));

}
