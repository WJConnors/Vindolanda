// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicBrick.h"


ABasicBrick::ABasicBrick()
{
	CreateBuilding();
}

void ABasicBrick::CreateBuilding()
{
	basicBrickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BasicBrickMesh"));
	RootComponent = basicBrickMesh;
}
