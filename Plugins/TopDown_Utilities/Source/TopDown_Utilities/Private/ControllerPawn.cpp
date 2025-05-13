// Fill out your copyright notice in the Description page of Project Settings.


#include "ControllerPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

// Sets default values
AControllerPawn::AControllerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = Cast<USceneComponent>(capsuleComponent);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(RootComponent);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);

	floatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));

}

// Called when the game starts or when spawned
void AControllerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AControllerPawn::Move(const FInputActionValue& value)
{
	const FVector2D movementInput = value.Get<FVector2D>();
	if (Controller)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		const FVector forward = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		const FVector right = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(forward, movementInput.Y);
		AddMovementInput(right, movementInput.X);
	}

}

void AControllerPawn::Zoom(const FInputActionValue& value)
{
	const float zoomDirection = value.Get<float>();
	if (Controller)
	{
		 float newWidth = camera->OrthoWidth + zoomDirection * zoomSpeed;
		 newWidth = FMath::Clamp(newWidth, minCameraWidth, maxCameraWidth);
		 camera->OrthoWidth = newWidth;
	}
}

// Called every frame
void AControllerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AControllerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (TObjectPtr<UEnhancedInputComponent> enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &AControllerPawn::Move);
		enhancedInputComponent->BindAction(zoomAction, ETriggerEvent::Triggered, this, &AControllerPawn::Zoom);
	}

}