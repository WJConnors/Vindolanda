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
	TSubclassOf<AActor> knightToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	FTransform knightSpawnPosition;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	float spawnInterval = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Building")
	AStaticMeshActor* townCentre{ nullptr };

	UFUNCTION(BlueprintCallable, Category = "Day")
	void SpawnKnight();

private:
	FTimerHandle spawnTimerHandle;

	void SpawnEnemy();

	bool bIsNight{ false };

	void BeginNight();

	void EndNight();

	UPROPERTY()
	TObjectPtr<AActor> lightingManager{ nullptr };

	FVector knightSpawnPositionDefunct{ 1716.213973 , -1844.396172, 60.968751 };
};
