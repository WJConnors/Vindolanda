// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBuilding.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

// Sets default values
ABaseBuilding::ABaseBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = staticMesh;

	selectedIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectedIndicator"));
	selectedIndicator->SetupAttachment(RootComponent);
	selectedIndicator->SetHiddenInGame(true);
	selectedIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ABaseBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseBuilding::SelectActor_Implementation(const bool select)
{
	selectedIndicator->SetHiddenInGame(!select);
}

void ABaseBuilding::EnablePlacing()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if (playerController)
	{
		EnableInput(playerController);
		UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(playerController->InputComponent);
		if (inputComponent)
		{
			inputComponent->BindAction(placeAction, ETriggerEvent::Completed, this, &ABaseBuilding::PlaceBuilding);
		}
	}
	GetWorld()->GetTimerManager().SetTimer(placementTimerHandle, this, &ABaseBuilding::CheckPlacementValidity, 0.03f, true);

	this->SetActorEnableCollision(false);
}

void ABaseBuilding::CheckPlacementValidity()
{
	FHitResult hit;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, hit);
	if (hit.bBlockingHit)
	{
		SetActorLocation(hit.Location);
	}
}

void ABaseBuilding::PlaceBuilding(const FInputActionValue& value)
{
	GetWorld()->GetTimerManager().ClearTimer(placementTimerHandle);
	this->SetActorEnableCollision(true);
}

void ABaseBuilding::CancelBuildingPlacement(const FInputActionValue& value)
{
}
