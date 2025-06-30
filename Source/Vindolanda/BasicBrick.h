// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseBuilding.h"
#include "BasicBrick.generated.h"

/**
 * 
 */
UCLASS()
class VINDOLANDA_API ABasicBrick : public ABaseBuilding
{
	GENERATED_BODY()

public:
	ABasicBrick();

protected:
	virtual void CreateBuilding() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> basicBrickMesh;
	
};
