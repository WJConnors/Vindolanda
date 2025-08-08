#include "DebugMsgBPLib.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UDebugMsgBPLib::ClientMessageBP(UObject* WorldContextObject, const FString& Message, float LifeTime)
{
	if (!WorldContextObject)
	{
		return;
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		PC->ClientMessage(Message, NAME_None, LifeTime);
	}
}

void UDebugMsgBPLib::ClientMessageForPC(APlayerController* PC, const FString& Message, float LifeTime)
{
	if (PC)
	{
		PC->ClientMessage(Message, NAME_None, LifeTime);
	}
}
