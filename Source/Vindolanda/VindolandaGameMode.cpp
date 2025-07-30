// Fill out your copyright notice in the Description page of Project Settings.


#include "VindolandaGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "NavigationSystem.h"

AVindolandaGameMode::AVindolandaGameMode()
{
}

void AVindolandaGameMode::BeginPlay()
{
	Super::BeginPlay();

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
	UE_LOG(LogTemp, Warning, TEXT("SPAWING"));
	UWorld* world = GetWorld();
	if (!world || !enemyToSpawn) return;
	UE_LOG(LogTemp, Warning, TEXT("PASSED FIRST CHECK"));

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
		world->SpawnActor<AActor>(
			enemyToSpawn,
			spawnLoc,
			FRotator::ZeroRotator,
			params
		);
	}
}
