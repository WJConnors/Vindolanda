// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "WorldBuilder.generated.h"

UCLASS()
class VINDOLANDA_API AWorldBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldBuilder();

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// The mesh asset to instance (set this to your brick mesh in the BP subclass)
	UPROPERTY(EditAnywhere, Category = "Bricks")
	UStaticMesh* BrickMesh;

	// Instanced mesh component that will batch‐draw all bricks
	UPROPERTY(VisibleAnywhere, Category = "Bricks")
	UHierarchicalInstancedStaticMeshComponent* InstancedBricks;

	// Grid cell size (in world units, e.g. 100 cm)
	UPROPERTY(EditAnywhere, Category = "Bricks")
	float CellSize = 100.f;

	// Half‐extent (so a value of 25 gives you –25..+25)
	UPROPERTY(EditAnywhere, Category = "Bricks")
	int32 HalfExtent = 25;
	

};
