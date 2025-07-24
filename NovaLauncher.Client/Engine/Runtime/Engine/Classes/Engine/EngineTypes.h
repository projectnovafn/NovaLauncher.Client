#pragma once

#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "UObject/Class.h"

enum class ELevelCollectionType : uint8
{
    DynamicSourceLevels,
    DynamicDuplicatedLevels,
    StaticLevels,
    MAX,
};

enum ENetRole
{
    ROLE_None,
    ROLE_SimulatedProxy,
    ROLE_AutonomousProxy,
    ROLE_Authority,
    ROLE_MAX,
};