#pragma once

#include "CoreUObject.h"
#include "Engine/EngineTypes.h"
#include "Logging/LogMacros.h"

class UWorld;
class ULevel;

class AActor : public UObject
{
	GENERATED_UCLASS_BODY(AActor, Engine)
private:

	UPROPERTY(AActor*, Owner)
	AActor* Owner;

public:

	AActor* GetOwner()
	{
		return Owner;
	}
};