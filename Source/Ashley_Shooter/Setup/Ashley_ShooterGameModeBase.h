// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Ashley_ShooterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ASHLEY_SHOOTER_API AAshley_ShooterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	public:
		AAshley_ShooterGameModeBase();
		virtual void StartPlay() override;
};
