// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicBrick.h"


ABasicBrick::ABasicBrick()
{
	CreateBasicBrick();
}

void ABasicBrick::CreateBasicBrick()
{
	basicBrickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BasicBrickMesh"));
	RootComponent = basicBrickMesh;
}
