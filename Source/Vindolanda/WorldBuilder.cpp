// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldBuilder.h"

// Sets default values
AWorldBuilder::AWorldBuilder()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create a root just to have a Transform, or reuse the default SceneComponent
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Create the instanced mesh component
	InstancedBricks = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("InstancedBricks"));
	InstancedBricks->SetupAttachment(Root);
	InstancedBricks->SetMobility(EComponentMobility::Static);
}

void AWorldBuilder::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Clear old instances so you can tweak HalfExtent / CellSize live
	InstancedBricks->ClearInstances();

	// Assign your mesh asset if it’s set
	if (BrickMesh)
	{
		InstancedBricks->SetStaticMesh(BrickMesh);
	}
	else
	{
		// No mesh means nothing to draw
		return;
	}

	// Spawn a flat grid from -HalfExtent..+HalfExtent in X/Y at Z=0
	const float Offset = HalfExtent * CellSize;
	for (int32 X = -HalfExtent; X <= HalfExtent; ++X)
	{
		for (int32 Y = -HalfExtent; Y <= HalfExtent; ++Y)
		{
			FVector Location = FVector(X * CellSize, Y * CellSize, 0.f);
			FTransform InstanceTransform = FTransform(Location);
			InstancedBricks->AddInstance(InstanceTransform);
		}
	}
}