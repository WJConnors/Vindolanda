#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DebugMsgBPLib.generated.h"

// Use your module's API macro (looks like VINDOLANDA_API in your project)
UCLASS()
class VINDOLANDA_API UDebugMsgBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// WorldContext lets you call this from anywhere in BP without wiring a world manually
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", DisplayName = "Client Message", ToolTip = "Print a small message top-left that works in packaged builds"), Category = "Debug|Message")
	static void ClientMessageBP(UObject* WorldContextObject, const FString& Message, float LifeTime = 3.f);

	// Optional: targeted version if you already have a PlayerController reference
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Client Message (Targeted)", ToolTip = "Same as Client Message but for a specific PlayerController"), Category = "Debug|Message")
	static void ClientMessageForPC(APlayerController* PC, const FString& Message, float LifeTime = 3.f);
};
