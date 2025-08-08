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
	State* runAway = new State([&]()->void {
		this->HomeTest();
		});
	State* taunted = new State([&]()->void {
		this->Taunted();
		});

	stateMachine->AddState(idle);
	stateMachine->AddState(goToTC);
	stateMachine->AddState(runAway);
	stateMachine->AddState(taunted);

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
			ShowGold();
			hasGold = true;
			return true;
		}
		return false;
		}));

	stateMachine->AddTransition(new StateTransition(goToTC, taunted, [&]()->bool {
		if (isTaunted)
		{
			prevState = goToTC;
			return true;
		}
		return false;
		}));

	stateMachine->AddTransition(new StateTransition(runAway, taunted, [&]()->bool {
		if (isTaunted)
		{
			prevState = runAway;
			return true;
		}
		return false;
		}));

	stateMachine->AddTransition(new StateTransition(taunted, goToTC, [&]()->bool {
		if (!isTaunted && prevState == goToTC)
		{
			MoveToLocation_Implementation(townCentre->GetActorLocation());
			return true;
		}
		return false;
		}));

	stateMachine->AddTransition(new StateTransition(taunted, goToTC, [&]()->bool {
		if (!isTaunted && prevState == runAway)
		{
			MoveToLocation_Implementation(spawnLoc);
			return true;
		}
		return false;
		}));

}

void ABaseEnemy::Taunted()
{
	if (!tauntedBy) return;

	const float StopRange = 100.f;
	const FVector MyLoc = GetActorLocation();
	const FVector TargetLoc = tauntedBy->GetActorLocation();

	// compare squared distances (cheaper than sqrt)
	const float DistSq = FVector::DistSquared(MyLoc, TargetLoc);
	
	if (DistSq > FMath::Square(StopRange))
	{
		MoveToLocation_Implementation(TargetLoc);
	}
	else
	{
		if (AAIController* AICon = Cast<AAIController>(GetController()))
		{
			AICon->StopMovement();
		}
	}
}

void ABaseEnemy::HomeTest()
{
	if (FVector::Dist(GetActorLocation(), spawnLoc) < StopDistance)
	{
		EndLife();
	}	
}

void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	stateMachine->Update();
}

void ABaseEnemy::Damage(float damage)
{
	health -= damage;
	if (health <= 0)
	{
		ReturnGold();
		EndLife();
	}
}

void ABaseEnemy::EndLife()
{
	Destroy();
}