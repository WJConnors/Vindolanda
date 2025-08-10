// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownGameModeBase.h"
#include "ResourceData.h"

#include "VindolandaGameMode.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class VINDOLANDA_API AVindolandaGameMode : public ATopDownGameModeBase
{
	GENERATED_BODY()
	
public:
	AVindolandaGameMode();

	UPROPERTY(BlueprintReadOnly, Category = "Resources")
	int32 Gold = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Resources")
	UDataTable* ResourceTable = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Resources")
	FName GoldRowName = TEXT("Gold");

	UFUNCTION(BlueprintImplementableEvent, Category = "Resources")
	void OnGoldChanged(int32 NewGold);

	UFUNCTION(BlueprintCallable, Category = "Resources")
	void SetGold(int32 NewAmount);

	UFUNCTION(BlueprintCallable, Category = "Resources")
	void AddGold(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Day")
	void BeginNight();

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> WidgetToToggle;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetWidgetToToggle(UUserWidget* InWidget) { WidgetToToggle = InWidget; }

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidgetToToggle(bool bShow) const;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> GoldWidget;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetGoldWidget(UUserWidget* InWidget) { GoldWidget = InWidget; }

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	TObjectPtr<UUserWidget> ToastWidget;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetToastWidget(UUserWidget* InWidget) { ToastWidget = InWidget; }

	UFUNCTION(BlueprintCallable, Category = "State")
	void EnemyKilled();

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsNight() { return bIsNight; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> enemyToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> knightToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	FTransform knightSpawnPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Building")
	AStaticMeshActor* townCentre{ nullptr };

	UFUNCTION(BlueprintCallable, Category = "Day", meta = (AllowPrivateAccess = "true"))
	void SpawnKnight();

	//Difficulty variables
	// Designed to be modified at the top of BeginNight before first use
	int wave{ 0 };

	float spawnTimer{ 10 };

	int baseNumToKill{ 5 };

	int numToKill{ 0 };

private:
	FTimerHandle spawnTimerHandle;

	void SpawnEnemy();

	bool bIsNight{ false };

	void EndNight();

	UPROPERTY()
	TObjectPtr<AActor> lightingManager{ nullptr };
};
