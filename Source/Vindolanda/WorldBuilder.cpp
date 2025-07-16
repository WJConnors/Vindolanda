#include "WorldBuilder.h"

AWorldBuilder::AWorldBuilder()
{
    PrimaryActorTick.bCanEverTick = false;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    InstancedBricks = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("InstancedBricks"));
    InstancedBricks->SetupAttachment(RootComponent);
    InstancedBricks->SetMobility(EComponentMobility::Static);
}

void AWorldBuilder::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (BrickMesh)
    {
        InstancedBricks->SetStaticMesh(BrickMesh);
    }
    else
    {
        return;
    }

    // Clear existing instances and maps
    InstancedBricks->ClearInstances();
    BrickMap.Empty();
    InstanceMap.Empty();

    // Populate initial floor
    PopulateFloor(halfExtentBP);
}

void AWorldBuilder::BeginPlay()
{
    Super::BeginPlay();

    ClearBricks();
    PopulateFloor(halfExtentRT);

    // Rebuild mapping for instances created in OnConstruction / loaded from the level
    BrickMap.Empty();
    InstanceMap.Empty();
    int32 InstanceCount = InstancedBricks->GetInstanceCount();
    for (int32 Index = 0; Index < InstanceCount; ++Index)
    {
        FTransform InstanceTransform;
        InstancedBricks->GetInstanceTransform(Index, InstanceTransform, true);
        FVector Location = InstanceTransform.GetLocation();
        FIntVector GridPos(
            FMath::RoundToInt(Location.X / CellSize),
            FMath::RoundToInt(Location.Y / CellSize),
            FMath::RoundToInt(Location.Z / CellSize)
        );
        BrickMap.Add(GridPos, Index);
        InstanceMap.Add(Index, GridPos);
    }
}

void AWorldBuilder::PopulateFloor(int32 halfExtent)
{
    for (int32 X = -halfExtent; X <= halfExtent; ++X)
    {
        for (int32 Y = -halfExtent; Y <= halfExtent; ++Y)
        {
            AddBrickAt(FIntVector(X, Y, 0));
        }
    }
}

bool AWorldBuilder::AddBrickAt(const FIntVector& GridPos)
{
    if (BrickMap.Contains(GridPos))
    {
        return false;
    }

    FVector WorldLocation = FVector(GridPos) * CellSize;
    FTransform InstanceTransform(WorldLocation);

    int32 NewIndex = InstancedBricks->AddInstance(InstanceTransform);
    BrickMap.Add(GridPos, NewIndex);
    InstanceMap.Add(NewIndex, GridPos);

    return true;
}

bool AWorldBuilder::RemoveBrickAt(const FIntVector& GridPos)
{
    int32* IndexPtr = BrickMap.Find(GridPos);
    if (!IndexPtr) return false;

    int32 RemoveIndex = *IndexPtr;
    int32 LastIndex = InstancedBricks->GetInstanceCount() - 1;

    // Remove the instance; this swaps the last into this slot
    InstancedBricks->RemoveInstance(RemoveIndex);

    if (RemoveIndex != LastIndex)
    {
        // Fix up the moved instance's mapping
        FIntVector MovedPos = InstanceMap[LastIndex];
        BrickMap[MovedPos] = RemoveIndex;
        InstanceMap.Add(RemoveIndex, MovedPos);
        InstanceMap.Remove(LastIndex);
    }
    // Remove mapping for removed instance
    BrickMap.Remove(GridPos);
    InstanceMap.Remove(RemoveIndex);

    return true;
}

void AWorldBuilder::ClearBricks()
{
    InstancedBricks->ClearInstances();
    BrickMap.Empty();
    InstanceMap.Empty();
}

bool AWorldBuilder::GetGridPositionFromLocation(const FVector& WorldLocation, FIntVector& OutGridPos) const
{
    // Convert world location to local space
    FVector Local = WorldLocation - GetActorLocation();
    OutGridPos.X = FMath::RoundToInt(Local.X / CellSize);
    OutGridPos.Y = FMath::RoundToInt(Local.Y / CellSize);
    OutGridPos.Z = FMath::RoundToInt(Local.Z / CellSize);
    return true;
}

bool AWorldBuilder::GetAdjacentGridPositionFromHit(const FHitResult& HitResult, FIntVector& OutAdjacentGridPos) const
{
    if (!HitResult.bBlockingHit)
    {
        return false;
    }

    // Base grid position
    FIntVector BaseGridPos;
    GetGridPositionFromLocation(HitResult.Location, BaseGridPos);

    // Determine face offset from impact normal
    FVector Normal = HitResult.ImpactNormal;
    int32 OffX = (Normal.X > 0.5f) ? 1 : (Normal.X < -0.5f ? -1 : 0);
    int32 OffY = (Normal.Y > 0.5f) ? 1 : (Normal.Y < -0.5f ? -1 : 0);
    int32 OffZ = (Normal.Z > 0.5f) ? 1 : (Normal.Z < -0.5f ? -1 : 0);
    FIntVector Offset(OffX, OffY, OffZ);

    OutAdjacentGridPos = BaseGridPos + Offset;
    return true;
}