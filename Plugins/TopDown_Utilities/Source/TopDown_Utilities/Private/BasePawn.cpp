// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/KismetMathLibrary.h"
#include "AIController.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = Cast<USceneComponent>(capsuleComponent);
	capsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	skeletalMesh->SetupAttachment(RootComponent);

	floatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));

	selectedIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectedIndicator"));
	selectedIndicator->SetupAttachment(RootComponent);
	selectedIndicator->SetHiddenInGame(true);
	selectedIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OrientToMovement();

}

void ABasePawn::OrientToMovement()
{
	if (!bMoving)
	{
		return;
	}

	FVector moveDirection{ moveTargetLocation - GetActorLocation() };
	if (moveDirection.Length() < stopDistance)
	{
		bMoving = false;
		return;
	}

	moveDirection.Normalize(1);
	//AddMovementInput(moveDirection, 1.f);

	FRotator finalRotation{ 0.0f, UKismetMathLibrary::MakeRotFromX(moveDirection).Yaw, 0.0f };

	FRotator newRotation = FMath::RInterpTo(GetActorRotation(), finalRotation, GetWorld()->GetDeltaSeconds(), rotateSpeed);

	SetActorRotation(newRotation);
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePawn::SelectActorLocal(const bool select)
{
	selectedIndicator->SetHiddenInGame(!select);
}

void ABasePawn::SelectActor_Implementation(const bool select)
{
	selectedIndicator->SetHiddenInGame(!select);
}

void ABasePawn::MoveToLocation_Implementation(const FVector targetLocation)
{
	moveTargetLocation = targetLocation + FVector(0, 0, GetDefaultHalfHeight());
	bMoving = true;

	AAIController* pawnAIController = Cast<AAIController>(GetController());
	pawnAIController->MoveToLocation(targetLocation, stopDistance);
}

