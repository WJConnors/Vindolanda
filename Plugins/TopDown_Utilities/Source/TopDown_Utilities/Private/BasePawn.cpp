// BasePawn.cpp

#include "BasePawn.h"
#include "Components/CapsuleComponent.h"     // <-- so UCapsuleComponent is a complete type
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Components/StaticMeshComponent.h"

ABasePawn::ABasePawn(const FObjectInitializer& ObjInit)
    : Super(ObjInit)
{
    UCapsuleComponent* Capsule = GetCapsuleComponent();
    check(Capsule);

    PrimaryActorTick.bCanEverTick = true;

    // configure CharacterMovementComponent as before…
    UCharacterMovementComponent* MoveComp = GetCharacterMovement();
    MoveComp->bOrientRotationToMovement = true;
    MoveComp->RotationRate = FRotator(0, 640.f, 0);
    MoveComp->SetMovementMode(MOVE_Walking);
    MoveComp->bUseRVOAvoidance = true;
    MoveComp->AvoidanceConsiderationRadius = 100.f;
    MoveComp->AvoidanceWeight = 0.5f;
    MoveComp->SetAvoidanceEnabled(true);

    SelectedIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectedIndicator"));
    SelectedIndicator->SetupAttachment(RootComponent);
    SelectedIndicator->SetHiddenInGame(true);
    SelectedIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABasePawn::BeginPlay()
{
    Super::BeginPlay();
}

void ABasePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // no manual OrientToMovement() needed—CharacterMovementComponent handles it
}

void ABasePawn::SelectActorLocal(bool bSelect)
{
    SelectedIndicator->SetHiddenInGame(!bSelect);
}

void ABasePawn::SelectActor_Implementation(bool bSelect)
{
    SelectActorLocal(bSelect);
}

void ABasePawn::MoveToLocation_Implementation(const FVector TargetLocation)
{
    if (AAIController* C = Cast<AAIController>(GetController()))
    {
        if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
        {
            FNavLocation NavLoc;
            FVector SearchExt(150.f, 150.f, 150.f);
            if (NavSys->ProjectPointToNavigation(TargetLocation, NavLoc, SearchExt))
            {
                C->MoveToLocation(NavLoc.Location, StopDistance);
            }
        }
    }
}

void ABasePawn::SetSelectable(bool b)
{
    bSelectable = b;
}

EPawnType ABasePawn::GetPawnType_Implementation()
{
    return pawnType;
}
