// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"

ABaseEnemy::ABaseEnemy()
{

}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	spawnLoc = GetActorLocation();

	stateMachine = new StateMachine();

	State* idle = new State();

	stateMachine->AddState(idle);
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	stateMachine->Update();
}

