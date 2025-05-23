// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "BasePawn.h"

ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = true;
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

	selectedActor = hit.GetActor();

	if (selectedActor)
	{

		if (selectedActor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{
			ISelectableInterface::Execute_SelectActor(selectedActor, true);
		}
	}
}

void ATopDownPlayerController::CommandAction(const FInputActionValue& value)
{
	if (selectedActor)
	{
		if (selectedActor->GetClass()->ImplementsInterface(UNavigableInterface::StaticClass()))
		{
			FHitResult hitResult;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, hitResult);
			if (hitResult.bBlockingHit)
			{
				INavigableInterface::Execute_MoveToLocation(selectedActor, hitResult.Location);
			}

		}
	}
}
