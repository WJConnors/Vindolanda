// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SelectableInterface.h"
#include "NavigableInterface.h"
#include "CommonENUMS.h"
#include "BasePawnInterface.h"

#include "BasePawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UFloatingPawnMovement;

UCLASS()
class TOPDOWN_UTILITIES_API ABasePawn : public APawn, public ISelectableInterface, public INavigableInterface, public IBasePawnInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> capsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> skeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> floatingPawnMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> selectedIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	EPawnType pawnType{ EPawnType::Villager };

	bool selectable{ true };

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OrientToMovement();

	FVector moveTargetLocation{ FVector::ZeroVector };
	bool bMoving{ false };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	float stopDistance{ 50.f };
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	float rotateSpeed{ 5.f };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void SelectActorLocal(const bool select);

	void SelectActor_Implementation(const bool select) override;

	void MoveToLocation_Implementation(const FVector targetLocation) override;

	EPawnType GetPawnType_Implementation() override;

	bool IsSelectable() const { return selectable; }

	void SetSelectable(bool b);

};
