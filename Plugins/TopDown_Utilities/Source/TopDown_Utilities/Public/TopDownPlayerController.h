// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ABasePawn;
struct FInputActionValue;

UCLASS()
class TOPDOWN_UTILITIES_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownPlayerController();

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> defaultInputMappingContext;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> selectAction;

	UPROPERTY()
	ABasePawn* selectedPawn;

protected:
	virtual void SetupInputComponent() override;

	void SelectAction(const FInputActionValue& value);
	
};
