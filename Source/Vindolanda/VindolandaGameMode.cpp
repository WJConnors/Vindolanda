// Fill out your copyright notice in the Description page of Project Settings.


#include "VindolandaGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

AVindolandaGameMode::AVindolandaGameMode()
{
}

void AVindolandaGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("TownCentre"), Found);
	if (Found.Num() > 0) townCentre = Cast<AStaticMeshActor>(Found[0]);

	if (enemyToSpawn)
	{
		GetWorldTimerManager().SetTimer(
			spawnTimerHandle,
			this,
			&AVindolandaGameMode::SpawnEnemy,
			spawnInterval,
			true,
			spawnInterval
		);
	}
}

void AVindolandaGameMode::SpawnEnemy()
{
	UWorld* world = GetWorld();
	if (!world || !enemyToSpawn) return;

	FVector origin(0, 0, 0);
	float radius = 1000.f;
	FNavLocation navLoc;

	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(world);
	if (navSys && navSys->GetRandomPointInNavigableRadius(origin, radius, navLoc))
	{
		FActorSpawnParameters params;
		params.Owner = this;
		params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector spawnLoc = navLoc.Location + FVector(0.f, 0.f, 90.f);
		ABaseEnemy* spawnedEnemy = Cast<ABaseEnemy>(world->SpawnActor<AActor>(
			enemyToSpawn,
			spawnLoc,
			FRotator::ZeroRotator,
			params
		));
		if (townCentre)
		{
			spawnedEnemy->SetTC(townCentre);
		}
	}
}
