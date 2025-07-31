// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "BaseEnemy.generated.h"

class StateMachine;

UCLASS()
class VINDOLANDA_API ABaseEnemy : public ABasePawn
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	virtual void Tick(float DeltaTime) override;

	void SetTC(AStaticMeshActor* targetActor) { townCentre = targetActor; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AStaticMeshActor> townCentre{ nullptr };

private:
	FVector spawnLoc;

	TObjectPtr<StateMachine> stateMachine;
	
};
