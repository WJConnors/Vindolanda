// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "BaseEnemy.generated.h"

class StateMachine;
class State;

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

	UFUNCTION(BlueprintCallable)
	bool GetIsTaunted() const { return isTaunted; }
	UFUNCTION(BlueprintCallable)
	void SetIsTaunted(bool b) { isTaunted = b; }

	UFUNCTION(BlueprintCallable)
	AActor* GetTauntedBy() const { return tauntedBy.Get(); }
	UFUNCTION(BlueprintCallable)
	void SetTauntedBy(AActor* a) { tauntedBy = a; }

	UFUNCTION(BlueprintImplementableEvent)
	void ShowGold();

	UFUNCTION(BlueprintImplementableEvent)
	void ReturnGold();

	UPROPERTY(BlueprintReadWrite)
	bool hasGold{ false };

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateKilled();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AStaticMeshActor> townCentre{ nullptr };

	float StopDistance{ 200.f };

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float goldSteal{ 10.f };

private:
	FVector spawnLoc;

	TObjectPtr<StateMachine> stateMachine;

	float health{ 100 };

	bool isTaunted{ false };

	State* prevState{ nullptr };

	void Taunted();

	TObjectPtr<AActor> tauntedBy{ nullptr };

	void HomeTest();

	void EndLife();
	
};
