#pragma once

#include "CoreMinimal.h"
#include "CommonENUMS.h"
#include "ResourceData.generated.h"


USTRUCT(BlueprintType)
struct FResourceData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	FString displayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	EResourceTypes resourceType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	TObjectPtr<UTexture2D> texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
	int32 defaultValue;

	FResourceData() : resourceType(EResourceTypes::Wood), displayName(TEXT("")), texture(nullptr), defaultValue(0) {}
	FResourceData(FString name, EResourceTypes type, UTexture2D* tex, int32 value)
		: displayName(name), resourceType(type), texture(tex), defaultValue(value) {
	}
};