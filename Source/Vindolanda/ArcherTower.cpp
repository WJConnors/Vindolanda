// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcherTower.h"

bool AArcherTower::PrePlacementRule(const FVector& PlacePos)
{
    if (FMath::Abs(PlacePos.X) >= 2300.f || FMath::Abs(PlacePos.Y) >= 2300.f)
    {
        bCanPlace = false;
        ToggleValidity(false);
        return true;
    }
    return false;
}