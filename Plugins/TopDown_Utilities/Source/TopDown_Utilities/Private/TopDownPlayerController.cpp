// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "BasePawn.h"
#include "TopDownHUD.h"


ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = true;
}

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();

	topDownHUD = Cast<ATopDownHUD>(GetHUD());
}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!defaultInputMappingContext) { return; }

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem) { Subsystem->AddMappingContext(defaultInputMappingContext, 0); }

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		enhancedInputComponent->BindAction(selectAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::SelectAction);

		enhancedInputComponent->BindAction(selectAction, ETriggerEvent::Started, this, &ATopDownPlayerController::SelectStart);
		enhancedInputComponent->BindAction(selectAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::SelectOngoing);
		enhancedInputComponent->BindAction(selectAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::SelectEnd);

		enhancedInputComponent->BindAction(commandAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::CommandAction);
	}

}

void ATopDownPlayerController::SelectAction(const FInputActionValue& value)
{
	FHitResult hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, hit);

	if (selectedActor)
	{
		if (selectedActor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{
			ISelectableInterface::Execute_SelectActor(selectedActor, false);
		}
	}

	AActor* hitActor = hit.GetActor();
	if (!hitActor)
	{
		selectedActor = nullptr;
		return;
	}

	if (ABasePawn* pawn = Cast<ABasePawn>(hitActor))
	{
		if (Cast<ABasePawn>(hit.GetActor())->IsSelectable())
		{
			selectedActor = hit.GetActor();

			if (selectedActor)
			{

				if (selectedActor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
				{
					ISelectableInterface::Execute_SelectActor(selectedActor, true);
				}
			}
		}
	}
	else
	{
		selectedActor = nullptr;
	}
}

void ATopDownPlayerController::CommandAction(const FInputActionValue& value)
{
	FHitResult hitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, hitResult);
	if (!hitResult.bBlockingHit) return;

	if (selectedActors.Num() > 0)
	{
		int i{ selectedActors.Num() / 2 };
		for (AActor* actor : allSelectedActors)
		{
			if (actor)
			{
				if (actor->GetClass()->ImplementsInterface(UNavigableInterface::StaticClass()))
				{
					INavigableInterface::Execute_MoveToLocation(actor, hitResult.Location + FVector(0, 100*i, 0));
					i++;
				}
			}
		}
	}

	else if(selectedActor)
	{
		if (selectedActor->GetClass()->ImplementsInterface(UNavigableInterface::StaticClass()))
		{
			INavigableInterface::Execute_MoveToLocation(selectedActor, hitResult.Location);
		}
	}
}

void ATopDownPlayerController::SelectStart(const FInputActionValue& value)
{
	float mouseX, mouseY;
	GetMousePosition(mouseX, mouseY);
	selectStartPos = FVector2D(mouseX, mouseY);
}

void ATopDownPlayerController::SelectOngoing(const FInputActionValue& value)
{
	float mouseX, mouseY;
	GetMousePosition(mouseX, mouseY);
	selectSize = FVector2D(mouseX - selectStartPos.X, mouseY - selectStartPos.Y);
	if (topDownHUD)
	{
		topDownHUD->ShowSelect(selectStartPos, selectSize);
	}
}

void ATopDownPlayerController::SelectEnd(const FInputActionValue& value)
{
	if (topDownHUD)
	{
		topDownHUD->HideSelect();
		FTimerHandle timerHandleSelectMultipleActors;
		GetWorld()->GetTimerManager().SetTimer(timerHandleSelectMultipleActors, this, &ATopDownPlayerController::SelectMultipleActors, .05f, false);
	}
}

void ATopDownPlayerController::SelectMultipleActors()
{
	if (topDownHUD)
	{
		for (AActor* actor : allSelectedActors)
		{
			if (actor)
			{
				if (actor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
				{
					ISelectableInterface::Execute_SelectActor(actor, false);
				}
			}
		}

		selectedActors.Empty();
		allSelectedActors = topDownHUD->GetselectedActors();

		for (AActor* actor : allSelectedActors)
		{
			if (actor)
			{
				if (actor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
				{
					if (Cast<ABasePawn>(actor)->IsSelectable())
					{
						ISelectableInterface::Execute_SelectActor(actor, true);
						selectedActors.AddUnique(actor);
					}
				}
			}
		}
		OnActorsSelected.Broadcast(selectedActors);
	}
}
