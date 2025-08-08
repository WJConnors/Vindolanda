// Fill out your copyright notice in the Description page of Project Settings.


#include "VindolandaGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "Blueprint/UserWidget.h"

AVindolandaGameMode::AVindolandaGameMode()
{
}

void AVindolandaGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> foundTC;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("TownCentre"), foundTC);
	if (foundTC.Num() > 0) townCentre = Cast<AStaticMeshActor>(foundTC[0]);



	TArray<AActor*> foundLM;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "LightingManager", foundLM);
	if (foundLM.Num() > 0)
	{
		lightingManager = foundLM[0];
	}
}

void AVindolandaGameMode::SpawnKnight()
{
	UWorld* world = GetWorld();
	if (!world || !knightToSpawn) return;

	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(world);
	if (!navSys) return;

	FNavLocation navLoc;
	FVector origin(0, 0, 0);
	float radius = 1500.f;

	navSys->GetRandomPointInNavigableRadius(origin, radius, navLoc);

	ABasePawn* knight = GetWorld()->SpawnActorDeferred<ABasePawn>(
		knightToSpawn, knightSpawnPosition, this, nullptr, 
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	if (!knight) return;

	UGameplayStatics::FinishSpawningActor(knight, knightSpawnPosition);

	knight->SpawnDefaultController();

	knight->MoveToLocation_Implementation(navLoc.Location);
}

void AVindolandaGameMode::SpawnEnemy()
{
	UWorld* world = GetWorld();
	if (!world || !enemyToSpawn) return;

	FVector origin(0, 0, 0);
	float radius = 15000.f;
	FNavLocation navLoc;

	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(world);

	if (!navSys) return;

	do {
		navSys->GetRandomPointInNavigableRadius(origin, radius, navLoc);
	} while (abs(navLoc.Location.X) < 11000 && abs(navLoc.Location.Y) < 11000);

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

void AVindolandaGameMode::EndNight()
{
	ShowWidgetToToggle(true);
}

void AVindolandaGameMode::BeginNight()
{
	if (!lightingManager) return;

	static const FName NAME_SetNight(TEXT("SetNight"));
	if (UFunction* Func = lightingManager->FindFunction(NAME_SetNight))
	{
		struct { bool bNight; } Params{ true };
		lightingManager->ProcessEvent(Func, &Params);
		bIsNight = true;
	}

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

	ShowWidgetToToggle(false);
}

int32 AVindolandaGameMode::GetGold() const
{
	if (!ResourceTable) return 0;
	const FResourceData* Row = ResourceTable->FindRow<FResourceData>(GoldRowName, TEXT("GetGold"));
	return Row ? Row->defaultValue : 0;
}

void AVindolandaGameMode::SetGold(int32 NewAmount)
{
	if (!ResourceTable) return;
	if (FResourceData* Row = ResourceTable->FindRow<FResourceData>(GoldRowName, TEXT("SetGold")))
	{
		Row->defaultValue = FMath::Max(0, NewAmount);
	}
}

void AVindolandaGameMode::AddGold(int32 Delta)
{
	if (!ResourceTable) return;
	if (FResourceData* Row = ResourceTable->FindRow<FResourceData>(GoldRowName, TEXT("AddGold")))
	{
		Row->defaultValue = FMath::Max(0, Row->defaultValue + Delta);
	}
}

void AVindolandaGameMode::ShowWidgetToToggle(bool bShow) const
{
	if (!WidgetToToggle) return;

	WidgetToToggle->SetVisibility(bShow ? ESlateVisibility::Visible
		: ESlateVisibility::Collapsed);
}