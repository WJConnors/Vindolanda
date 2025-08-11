#include "VindolandaSaveLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "VindolandaSaveGame.h"

namespace {
	static const FString SlotName = TEXT("PlayerProgress");
	static const int32   UserIndex = 0;

	static UVindolandaSaveGame* LoadOrCreate(UObject* WorldContext)
	{
		UWorld* W = GEngine ? GEngine->GetWorldContexts()[0].World() : nullptr;
		UObject* Ctx = WorldContext ? WorldContext : W;
		if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
			return Cast<UVindolandaSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex));
		auto* SG = Cast<UVindolandaSaveGame>(UGameplayStatics::CreateSaveGameObject(UVindolandaSaveGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SG, SlotName, UserIndex);
		return SG;
	}
}

void UVindolandaSaveLibrary::UpdateHighestRound(int32 NewRound)
{
	auto* SG = LoadOrCreate(nullptr);
	if (NewRound > SG->HighestRound)
	{
		SG->HighestRound = NewRound;
		UGameplayStatics::AsyncSaveGameToSlot(SG, SlotName, UserIndex);
	}
}

int32 UVindolandaSaveLibrary::GetHighestRound()
{
	return LoadOrCreate(nullptr)->HighestRound;
}