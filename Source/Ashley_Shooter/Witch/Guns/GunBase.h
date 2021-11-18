// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

UCLASS(Blueprintable)
class ASHLEY_SHOOTER_API AGunBase : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
		class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Gun")
		class USceneComponent* DefaultGunMuzzleLocation;

public:	
	// Sets default values for this actor's properties
	AGunBase();

};
