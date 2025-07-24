#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"
#include "EngineTypes.h"

#include "GameInstance.h"

class UNetDriver;
class AGameStateBase;
class AGameModeBase;

class FNetworkNotify
{
	
};

class UWorld final : public UObject, public FNetworkNotify
{
	GENERATED_UCLASS_BODY(UWorld, Engine)
public:

	UPROPERTY(UGameInstance*, OwningGameInstance)
	UGameInstance* GameInstance;
};

inline UWorld* GWorld = NULL;