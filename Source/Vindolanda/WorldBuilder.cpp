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
    PopulateDefaultWalls(halfExtentRTMax, halfExtentRTMin);
    //PopulateFloor(halfExtentRTMax, halfExtentRTMin);
}

void AWorldBuilder::BeginPlay()
{
    Super::BeginPlay();

    ClearBricks();
    PopulateDefaultWalls(halfExtentRTMax, halfExtentRTMin);
    //PopulateFloor(halfExtentRTMax, halfExtentRTMin);

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
    GetWorld()->Exec(GetWorld(), TEXT("RebuildNavigation"));
}

void AWorldBuilder::PopulateFloor(int32 halfExtentMax, int32 halfExtentMin)
{
    for (int32 x = -halfExtentMax; x <= halfExtentMax; ++x)
    {
        for (int32 y = -halfExtentMax; y <= halfExtentMax; ++y)
        {
            if ((x < halfExtentMin && x > -halfExtentMin) && (y < halfExtentMin && y > -halfExtentMin)) continue;
            AddBrickAt(FIntVector(x, y, 0));
        }
    }
}

void AWorldBuilder::PopulateDefaultWalls(int32 halfExtentMax, int32 halfExtentMin)
{
    for (int32 i = 0; i < 5; i++)
    {
        // Top and bottom edges
        for (int32 X = -halfExtentMax; X <= halfExtentMax; ++X)
        {
            if (X < -66 || (X > -52 && X < 40) || X > 70) AddBrickAt(FIntVector(X, halfExtentMax, i), true);
            if (X < -80 || (X > -44 && X < 46) || X > 74) AddBrickAt(FIntVector(X, -halfExtentMax, i), true);
        }
        // Left and right edges (excluding corners)
        for (int32 Y = -halfExtentMax + 1; Y < halfExtentMax; ++Y)
        {
            if (Y < -12 || Y > 25) AddBrickAt(FIntVector(halfExtentMax, Y, i), true);
            if (Y < 60 || Y > 90) AddBrickAt(FIntVector(-halfExtentMax, Y, i), true);
        }
        // Top and bottom edges
        for (int32 X = -halfExtentMin; X <= halfExtentMin; ++X)
        {
            if (X > -3 && X < 3) continue;
            AddBrickAt(FIntVector(X, halfExtentMin, i), true);
            AddBrickAt(FIntVector(X, -halfExtentMin, i), true);
        }
        // Left and right edges (excluding corners)
        for (int32 Y = -halfExtentMin + 1; Y < halfExtentMin; ++Y)
        {
            if (Y > -3 && Y < 3) continue;
            AddBrickAt(FIntVector(halfExtentMin, Y, i), true);
            AddBrickAt(FIntVector(-halfExtentMin, Y, i), true);
        }
    }

}

bool AWorldBuilder::AddBrickAt(const FIntVector& GridPos, bool init)
{
    if (BrickMap.Contains(GridPos)) return false;

    if (!init && !IsValidBuild(GridPos)) return false;

    FVector WorldLocation = FVector(GridPos) * CellSize;
    FTransform InstanceTransform(WorldLocation);

    int32 NewIndex = InstancedBricks->AddInstance(InstanceTransform);
    BrickMap.Add(GridPos, NewIndex);
    InstanceMap.Add(NewIndex, GridPos);
    GetWorld()->Exec(GetWorld(), TEXT("RebuildNavigation"));

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
        if (FIntVector* MovedGridPos = InstanceMap.Find(LastIndex))
        {
            // Update both maps to point that GridPos at the new index
            BrickMap.Add(*MovedGridPos, RemoveIndex);
            InstanceMap.Add(RemoveIndex, *MovedGridPos);
        }
    }

    // Remove mapping for removed instance
    BrickMap.Remove(GridPos);
    InstanceMap.Remove(LastIndex);
    GetWorld()->Exec(GetWorld(), TEXT("RebuildNavigation"));

    return true;
}

void AWorldBuilder::ClearBricks()
{
    InstancedBricks->ClearInstances();
    BrickMap.Empty();
    InstanceMap.Empty();
}

bool AWorldBuilder::GetGridPositionFromLocation(const FVector& WorldLocation, FIntVector& OutGridPos, bool isNewFloor) const
{
    // Convert world location to local space
    FVector Local = WorldLocation - GetActorLocation();
    OutGridPos.X = FMath::RoundToInt(Local.X / CellSize);
    OutGridPos.Y = FMath::RoundToInt(Local.Y / CellSize);
    OutGridPos.Z = isNewFloor ? 0 : FMath::RoundToInt(Local.Z / CellSize);
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

bool AWorldBuilder::IsValidBuild(const FIntVector& GridPos) const
{
    // Only consider X/Y for the walls
    const int32 X = GridPos.X;
    const int32 Y = GridPos.Y;
    const int32 AbsX = FMath::Abs(X);
    const int32 AbsY = FMath::Abs(Y);

    // 1) Outside the outer wall?  Reject.
    if (AbsX >= halfExtentRTMax || AbsY >= halfExtentRTMax)
    {
        return false;
    }

    // 2) Inside the inner wall?  (i.e. fully enclosed by it)  Reject.
    //    We only reject if *both* X and Y lie within the smaller square.
    if (AbsX <= halfExtentRTMin && AbsY <= halfExtentRTMin)
    {
        return false;
    }

    // 3) Otherwise it lies between the two walls.  Accept.
    return true;
}