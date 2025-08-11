#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VindolandaSaveGame.generated.h"

UCLASS()
class UVindolandaSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int32 HighestRound = 0;
};