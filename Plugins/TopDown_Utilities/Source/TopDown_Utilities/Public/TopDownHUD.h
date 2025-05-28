// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_UTILITIES_API ATopDownHUD : public AHUD
{
	GENERATED_BODY()

private:
	bool bDrawSelectionRect{ false };
	bool bSelectActors{ false };
	FVector2D selectStart;
	FVector2D selectSize;
	FLinearColor selectColor{ FLinearColor(0.f, 0.f, 1.f, .2f) };

	TArray<AActor*> selectedActors;

	void SelectActorsInRect();

protected:
	virtual void DrawHUD() override;

public:
	void ShowSelect(const FVector2D inSelectStart, const FVector2D inSelectSize);
	void HideSelect();

	TArray<AActor*> GetselectedActors();
};
