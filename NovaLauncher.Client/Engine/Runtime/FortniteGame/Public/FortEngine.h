#pragma once

#include "Engine.h"

class UFortEngine : public UGameEngine
{
	GENERATED_UCLASS_BODY(UFortEngine, FortniteGame)

public:
	void Init(void* EngineLoop);
};