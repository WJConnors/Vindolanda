// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ABasePawn;
class ATopDownHUD;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> commandAction;

	UPROPERTY()
	TObjectPtr<AActor> selectedActor;

	UPROPERTY()
	TObjectPtr<ATopDownHUD> topDownHUD;

	FVector2D selectStartPos;
	FVector2D selectSize;

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void SelectAction(const FInputActionValue& value);

	void CommandAction(const FInputActionValue& value);

	void SelectStart(const FInputActionValue& value);
	void SelectOngoing(const FInputActionValue& value);
	void SelectEnd(const FInputActionValue& value);
	
};
