// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

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

}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePawn::SelectActor(const bool select)
{
	selectedIndicator->SetHiddenInGame(!select);
}

