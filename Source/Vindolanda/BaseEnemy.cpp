// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"
#include "Engine/StaticMeshActor.h"
#include "AIController.h"

ABaseEnemy::ABaseEnemy()
{
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	SetSelectable(false);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetSelectable(false);

	if (!GetController()) SpawnDefaultController();

	spawnLoc = GetActorLocation();

	stateMachine = new StateMachine();

	State* idle = new State();
	State* goToTC = new State();
	State* runAway = new State();

	stateMachine->AddState(idle);
	stateMachine->AddState(goToTC);
	stateMachine->AddState(runAway);

	stateMachine->AddTransition(new StateTransition(idle, goToTC, [&]()->bool {
		if (townCentre && GetController())
		{
			MoveToLocation_Implementation(townCentre->GetActorLocation());
			return true;
		}
		return false;
		}));
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	stateMachine->Update();
}

