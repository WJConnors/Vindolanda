// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arrow.generated.h"

UCLASS()
class VINDOLANDA_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

	UFUNCTION(BlueprintCallable, Category = "Arrow")
	void SetTarget(AActor* newTarget) { target = newTarget; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "Arrow")
	TObjectPtr<AActor> target{ nullptr };

};
