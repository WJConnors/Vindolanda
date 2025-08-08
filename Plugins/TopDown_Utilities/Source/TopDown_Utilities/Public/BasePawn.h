// BasePawn.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SelectableInterface.h"
#include "NavigableInterface.h"
#include "CommonENUMS.h"
#include "BasePawnInterface.h"
#include "BasePawn.generated.h"

UCLASS()
class TOPDOWN_UTILITIES_API ABasePawn
    : public ACharacter
    , public ISelectableInterface
    , public INavigableInterface
    , public IBasePawnInterface
{
    GENERATED_BODY()

public:
    ABasePawn(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // ISelectableInterface
    UFUNCTION()
    void SelectActorLocal(bool bSelect);

    void SelectActor_Implementation(bool bSelect) override;

    // INavigableInterface
    

    // IBasePawnInterface
    EPawnType GetPawnType_Implementation() override;

    

public:
    /** How close before we consider the move “complete.” */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
    float StopDistance = 50.f;

    /** Show/hide selection indicator */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn")
    UStaticMeshComponent* SelectedIndicator;

    /** Which type of pawn am I? */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
    EPawnType pawnType{ EPawnType::Villager };

    bool IsSelectable() const override { return bSelectable; }
    void SetSelectable(bool b);  // no 'override'—not in any base class

    UFUNCTION(BlueprintCallable, Category = "AI")
    void MoveToLocation_Implementation(const FVector TargetLocation) override;

private:
    bool bSelectable = true;
};
