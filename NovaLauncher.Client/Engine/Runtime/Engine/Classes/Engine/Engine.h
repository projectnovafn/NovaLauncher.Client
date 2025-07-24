#pragma once

#include "CoreMinimal.h"
#include "CoreUObject.h"

#include "KismetMemoryLibrary.h"
#include "GameInstance.h"

class UNetDriver;
class UWorld;

class UEngine : public UObject
{
	GENERATED_UCLASS_BODY(UEngine, Engine)

public:

	UPROPERTY(UGameInstance*, GameInstance)
	UGameInstance* GameInstance;
};

inline UEngine* GEngine;