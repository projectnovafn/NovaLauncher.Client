#pragma once
#include "FortEngine.h"

class UFortEnginePatches
{
	static void InitHook(UFortEngine*, void*);
public:
	static void Init();
};