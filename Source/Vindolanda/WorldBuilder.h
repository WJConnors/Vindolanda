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
    AWorldBuilder();

    /** Adds a brick at GridPos if none exists. Returns true if added. */
    UFUNCTION(BlueprintCallable, Category = "Bricks")
    bool AddBrickAt(const FIntVector& GridPos);

    /** Removes the brick at GridPos if exists. Returns true if removed. */
    UFUNCTION(BlueprintCallable, Category = "Bricks")
    bool RemoveBrickAt(const FIntVector& GridPos);

    /** Clears all bricks */
    UFUNCTION(BlueprintCallable, Category = "Bricks")
    void ClearBricks();

    /** Converts a world location to the nearest grid coordinate */
    UFUNCTION(BlueprintCallable, Category = "Bricks")
    bool GetGridPositionFromLocation(const FVector& WorldLocation, FIntVector& OutGridPos, bool isNewFloor = false) const;

    /**
     * Given a HitResult from a cursor-trace, returns the grid coordinate adjacent to the hit face.
     * Returns true if successful, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Bricks")
    bool GetAdjacentGridPositionFromHit(const FHitResult& HitResult, FIntVector& OutAdjacentGridPos) const;

protected:
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void BeginPlay() override;

    // Instanced mesh for bricks
    UPROPERTY(VisibleAnywhere, Category = "Bricks")
    UHierarchicalInstancedStaticMeshComponent* InstancedBricks;

    // Mesh asset to instance
    UPROPERTY(EditAnywhere, Category = "Bricks")
    UStaticMesh* BrickMesh;

    // Grid cell size
    UPROPERTY(EditAnywhere, Category = "Bricks")
    float CellSize = 100.f;

    // Half extent
    UPROPERTY(EditAnywhere, Category = "Bricks")
    int32 halfExtentBP = 25;

    UPROPERTY(EditAnywhere, Category = "Bricks")
    int32 halfExtentRTMax = 40;

    UPROPERTY(EditAnywhere, Category = "Bricks")
    int32 halfExtentRTMin = 25;

private:
    // Mapping from grid pos to instance index
    TMap<FIntVector, int32> BrickMap;

    //Quick lookup of which grid‐coordinate lives at a given instance index. 
    TMap<int32, FIntVector> InstanceMap;

    void PopulateFloor(int32 halfExtentMax, int32 halfExtentMin);
    void PopulateDefaultWalls(int32 halfExtentMax, int32 halfExtentMin);
};