// Fill out your copyright notice in the Description page of Project Settings.


#include "Witch.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Guns/GunBase.h"
#include "Movement/PlayerMovementComponent.h"

// Sets default values
AWitch::AWitch(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurnRate = 30.0f;
	LookUpRate = 30.0f;

	USkeletalMeshComponent* TPSMesh = GetMesh();
	TPSMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
	TPSMesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	// The owning player doesn't see the regular (third-person) body mesh.
	TPSMesh->SetOwnerNoSee(true);

	CameraSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	check(CameraSpringArmComp != nullptr);
	CameraSpringArmComp->SetupAttachment(GetCapsuleComponent());
	CameraSpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	CameraSpringArmComp->TargetArmLength = 0.0f;

	// Create a first person camera component.
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	check(CameraComponent != nullptr);
	// Attach the camera component to our capsule component.
	CameraComponent->SetupAttachment(CameraSpringArmComp, USpringArmComponent::SocketName);
	CameraComponent->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
	// Enable the pawn to control camera rotation.
	CameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);
	// Only the owning player sees this mesh.
	FPSMesh->SetOnlyOwnerSee(true);
	// Attach the FPS mesh to the FPS camera.
	FPSMesh->SetupAttachment(CameraComponent);
	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	FPSMesh->SetRelativeLocation(FVector(3.293318f, -4.998758f, -161.327179f));
	FPSMesh->SetRelativeRotation(FRotator(1.899995f, -19.189999f, 5.200002f));
}

// Called when the game starts or when spawned
void AWitch::BeginPlay()
{
	Super::BeginPlay();

	if (GunClass) {
		FActorSpawnParameters GunSpawnParams;
		GunSpawnParams.bNoFail = true;
		GunSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform GunTransform;
		GunTransform.SetLocation(FVector::ZeroVector);
		GunTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		Gun = GetWorld()->SpawnActor<AGunBase>(GunClass, GunTransform, GunSpawnParams);
		if (Gun) {
			Gun->AttachToComponent(FPSMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
		}
	}
	

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//
	
	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using AWitch."));
}

// Called every frame
void AWitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWitch::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AWitch::MoveForward);
	PlayerInputComponent->BindAxis("Strafe", this, &AWitch::MoveRight);
	// Set up "look" bindings. Functions provided by ACharacter Class
	PlayerInputComponent->BindAxis("Turn", this, &AWitch::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AWitch::LookUp);
	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AWitch::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AWitch::StopJump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AWitch::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AWitch::StopSprinting);
}

void AWitch::MoveForward(float Value)
{
    // Find out which way is "forward" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    AddMovementInput(Direction, Value);
}

void AWitch::MoveRight(float Value)
{
    // Find out which way is "right" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value);
}

void AWitch::Turn(float Value)
{
	AddControllerYawInput(Value * TurnRate * GetWorld()->GetDeltaSeconds());
}

void AWitch::LookUp(float Value)
{
	AddControllerPitchInput(Value * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void AWitch::StartJump()
{
    bPressedJump = true;
}

void AWitch::StopJump()
{
    bPressedJump = false;
}

void AWitch::StartSprinting()
{
	UPlayerMovementComponent* MoveComp = Cast<UPlayerMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->SetSprinting(true);
	}
}

void AWitch::StopSprinting()
{
	UPlayerMovementComponent* MoveComp = Cast<UPlayerMovementComponent>(GetCharacterMovement());
	if (MoveComp)
	{
		MoveComp->SetSprinting(false);
	}
}
