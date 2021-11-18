// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"

// Sets default values
AGunBase::AGunBase()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DefaultGunMeshComp"));
	MeshComp->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	MeshComp->bCastDynamicShadow = false;
	MeshComp->CastShadow = false;
	RootComponent = MeshComp;

	DefaultGunMuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultGunMuzzleLocation"));
	DefaultGunMuzzleLocation->SetupAttachment(MeshComp);
	DefaultGunMuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.400002f, -10.6f));
}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();


}
