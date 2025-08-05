// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBuilding.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Engine/EngineTypes.h"
#include "Engine/OverlapResult.h"


// Sets default values
ABaseBuilding::ABaseBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetClass() == ABaseBuilding::StaticClass())
	{
		CreateBuilding();
	}
}

void ABaseBuilding::CreateBuilding()
{
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = staticMesh;
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
	// 1) Project mouse onto floor plane
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	FVector WorldOrigin, WorldDir;
	if (!PC->DeprojectMousePositionToWorld(WorldOrigin, WorldDir))
		return;

	const float FloorZ = 0.f;  // adjust if your ground isn't at Z=0
	float T = (FloorZ - WorldOrigin.Z) / WorldDir.Z;
	FVector GroundPos = WorldOrigin + WorldDir * T;

	// 2) Compute mesh bottom offset
	FVector LocalMin, LocalMax;
	float BottomOffset = bottomOffset;
	if (staticMesh)
	{
		staticMesh->GetLocalBounds(LocalMin, LocalMax);
		BottomOffset = -LocalMin.Z;
	}

	// 3) Position preview so mesh bottom sits on the floor
	FVector PlacePos = GroundPos + FVector(0.f, 0.f, BottomOffset);
	SetActorLocation(PlacePos);

	// 4) Overlap test against world static objects (ignoring floor)
	FVector HalfExt = buildingExtents * 0.5f;
	FCollisionShape BoxShape = FCollisionShape::MakeBox(HalfExt);
	FCollisionQueryParams QParams;
	QParams.AddIgnoredActor(this);

	TArray<FOverlapResult> OutResults;
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OutResults,
		PlacePos,
		FQuat::Identity,
		ECC_WorldStatic,
		BoxShape,
		QParams
	);

	bool bCollidesInvalid = false;
	for (const FOverlapResult& R : OutResults)
	{
		if (AActor* Other = R.GetActor())
		{
			// skip floor (ensure your floor actor has this tag)
			if (!Other->ActorHasTag(requiredTag))
			{
				bCollidesInvalid = true;
				break;
			}
		}
	}

	bCanPlace = !bCollidesInvalid;
	ToggleValidity(bCanPlace);
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
