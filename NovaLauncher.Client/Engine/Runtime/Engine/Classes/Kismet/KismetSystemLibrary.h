#pragma once

#include "CoreUObject.h"

class UKismetSystemLibrary : public UObject
{
    GENERATED_UCLASS_BODY(UKismetSystemLibrary, Engine)

public:

    static FString GetEngineVersion();

    static double GetEngineVersionNum()
    {
        auto EngineVersion = GetEngineVersion().ToString();
        return std::stod(EngineVersion.substr(0, 4));
    }

    static double GetFortniteVersion()
    {
        auto EngineVersion = GetEngineVersion().ToString();
        return std::stod(EngineVersion.substr(EngineVersion.find_last_of("-") + 1, EngineVersion.length() - EngineVersion.find_last_of("-") + 1));
    }
};