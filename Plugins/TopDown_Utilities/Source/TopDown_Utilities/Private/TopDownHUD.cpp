// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownHUD.h"

void ATopDownHUD::DrawHUD()
{
	Super::DrawHUD();

	if (bDrawSelectionRect)
	{
		DrawRect(selectColor, selectStart.X, selectStart.Y, selectSize.X, selectSize.Y);
	}
}

void ATopDownHUD::ShowSelect(const FVector2D inSelectStart, const FVector2D inSelectSize)
{
	selectStart = inSelectStart;
	selectSize = inSelectSize;
	bDrawSelectionRect = true;
}

void ATopDownHUD::HideSelect()
{
	bDrawSelectionRect = false;
}
