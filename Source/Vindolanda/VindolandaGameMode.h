// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownGameModeBase.h"
#include "VindolandaGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VINDOLANDA_API AVindolandaGameMode : public ATopDownGameModeBase
{
	GENERATED_BODY()
	
public:
	AVindolandaGameMode();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> enemyToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	float spawnInterval = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Building")
	AStaticMeshActor* townCentre{ nullptr };

private:
	FTimerHandle spawnTimerHandle;

	void SpawnEnemy();
};
