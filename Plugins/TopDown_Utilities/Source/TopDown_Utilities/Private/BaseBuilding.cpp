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
	selectedIndicator->SetRelativeLocation(FVector(0.f, 0.f, -100.f));

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

	ToggleValidity(bCanPlace);

	bCanPlace = false;
	FHitResult hit;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECC_Visibility, false, hit);
	if (!hit.bBlockingHit)
	{
		return;
	}

	FVector modifiedLocation{ hit.Location + FVector(0.f, 0.f, 100.f) };
	SetActorLocation(modifiedLocation);

	if (!hit.GetActor()->ActorHasTag(requiredTag))
	{
		return;
	}

	FVector boxHalfExtent = buildingExtents / 2;
	FVector traceStart = modifiedLocation + FVector(0.f, 0.f, boxHalfExtent.Z);
	FVector traceEnd = traceStart + FVector::UpVector;

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(this);
	collisionParams.bTraceComplex = false;
	TArray<FHitResult> outHits;

	bool boxHit = GetWorld()->SweepMultiByChannel(outHits, traceStart, traceEnd, GetActorRotation().Quaternion(), ECC_Visibility, FCollisionShape::MakeBox(boxHalfExtent), collisionParams);

	for (const FHitResult& bHit : outHits)
	{
		if (!(bHit.GetActor() != nullptr && bHit.GetActor()->ActorHasTag(requiredTag)))
		{
			break;
		}
	}

	bCanPlace = true;
	
}

void ABaseBuilding::PlaceBuilding(const FInputActionValue& value)
{
	if (!bCanPlace)
	{
		SetLifeSpan(0.1f);
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(placementTimerHandle);
	this->SetActorEnableCollision(true);
}

void ABaseBuilding::CancelBuildingPlacement(const FInputActionValue& value)
{
}
