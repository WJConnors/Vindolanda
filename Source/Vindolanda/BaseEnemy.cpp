// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "StateMachine.h"
#include "StateTransition.h"
#include "State.h"
#include "Engine/StaticMeshActor.h"
#include "AIController.h"
#include "DetourCrowdAIController.h"

ABaseEnemy::ABaseEnemy(const FObjectInitializer& ObjInit)
	: Super(ObjInit)   // this runs ABasePawn(const FObjectInitializer&) then ACharacter
{
	// now you can safely tweak AIControllerClass etc.
	AIControllerClass = ADetourCrowdAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	SetSelectable(false);
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

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

	stateMachine->AddTransition(new StateTransition(goToTC, runAway, [&]()->bool {
		if (FVector::Dist(GetActorLocation(), townCentre->GetActorLocation()) < StopDistance)
		{
			MoveToLocation_Implementation(spawnLoc);
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

void ABaseEnemy::Damage(float damage)
{
	health -= damage;
	if (health <= 0) Destroy();
}