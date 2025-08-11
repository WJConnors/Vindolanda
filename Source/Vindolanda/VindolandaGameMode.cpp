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

	int32 DefaultGold = 200; // fallback
	if (ResourceTable)
	{
		if (const FResourceData* Row = ResourceTable->FindRow<FResourceData>(GoldRowName, TEXT("Init")))
		{
			DefaultGold = Row->defaultValue;
		}
	}
	Gold = DefaultGold;
	OnGoldChanged(Gold);
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
	GetWorldTimerManager().ClearTimer(spawnTimerHandle);
	spawnTimerHandle.Invalidate();

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), Enemies);
	for (AActor* A : Enemies)
	{
		if (A) { A->Destroy(); }
	}

	static const FName NAME_SetNight(TEXT("SetNight"));
	if (UFunction* Func = lightingManager->FindFunction(NAME_SetNight))
	{
		struct { bool bNight; } Params{ false };
		lightingManager->ProcessEvent(Func, &Params);
		bIsNight = false;
	}
}

void AVindolandaGameMode::BeginNight()
{
	if (!lightingManager) return;

	wave++;
	spawnTimer /= 2;
	baseNumToKill *= 2;
	numToKill = baseNumToKill;

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
			spawnTimer,
			true,
			spawnTimer
		);
	}

	ShowWidgetToToggle(false);
}

void AVindolandaGameMode::SetGold(int32 NewAmount)
{
	Gold = FMath::Max(0, NewAmount);
	OnGoldChanged(Gold);        
}

void AVindolandaGameMode::AddGold(int32 Delta)
{
	Gold += Delta;
	bool gameOver = Gold < 0;


	Gold = FMath::Max(0, Gold);
	OnGoldChanged(Gold);

	if (gameOver)
	{
		//UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
		const FName LevelName(*UGameplayStatics::GetCurrentLevelName(this, true));
		UGameplayStatics::OpenLevel(this, LevelName);
	}
}

void AVindolandaGameMode::ShowWidgetToToggle(bool bShow) const
{
	if (!WidgetToToggle) return;

	WidgetToToggle->SetVisibility(bShow ? ESlateVisibility::Visible
		: ESlateVisibility::Collapsed);
}

void AVindolandaGameMode::EnemyKilled()
{
	numToKill--;
	if (numToKill <= 0) EndNight();
}