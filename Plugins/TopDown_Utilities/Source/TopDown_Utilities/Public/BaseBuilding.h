// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectableInterface.h"

#include "BaseBuilding.generated.h"

class UBoxComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TOPDOWN_UTILITIES_API ABaseBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseBuilding();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> placeAction;

	UPROPERTY()
	FTimerHandle placementTimerHandle;

	UPROPERTY()
	FName requiredTag{ "CanPlaceBuildings" };

	UPROPERTY()
	bool bCanPlace{ false };

	UPROPERTY()
	FVector buildingExtents{ FVector(500.5, 500.f, 500.f) };

	virtual void CreateBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Building")
	void EnablePlacing();

	void CheckPlacementValidity();

	void PlaceBuilding(const FInputActionValue& value);

	void CancelBuildingPlacement(const FInputActionValue& value);

	UFUNCTION(BlueprintImplementableEvent, Category = "Building")
	void ToggleValidity(bool valid);


};
