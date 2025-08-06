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
	ABaseEnemy(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

	virtual void Tick(float DeltaTime) override;

	void SetTC(AStaticMeshActor* targetActor) { townCentre = targetActor; }

	UFUNCTION(BlueprintCallable)
	void Damage(float damage);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AStaticMeshActor> townCentre{ nullptr };

	float StopDistance{ 200.f };

private:
	FVector spawnLoc;

	TObjectPtr<StateMachine> stateMachine;

	float health{ 100 };
	
};
