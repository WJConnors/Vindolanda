// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPlayerController.h"
#include "EnhancedInputSubsystems.h"

ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = true;
}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!defaultInputMappingContext) { return; }

	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem) { Subsystem->AddMappingContext(defaultInputMappingContext, 0); }

}