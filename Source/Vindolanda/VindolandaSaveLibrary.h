#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VindolandaSaveLibrary.generated.h"

UCLASS()
class UVindolandaSaveLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Save")
	static void UpdateHighestRound(int32 NewRound);

	UFUNCTION(BlueprintPure, Category = "Save")
	static int32 GetHighestRound();
};