#include "Patches/FortEnginePatch.h"
#include "KismetDetoursLibrary.h"

static void (*_Init)(UFortEngine*, void*);

UDetour* InitDetour = NULL;

void UFortEnginePatches::InitHook(UFortEngine* Engine, void* EngineLoop)
{
	Engine->Init(EngineLoop);

	_Init(Engine, EngineLoop);

	InitDetour->Remove();
}

void UFortEnginePatches::Init()
{
	_Init = UKismetMemoryLibrary::Get<decltype(_Init)>(L"UFortEngine::Init");

	InitDetour = new UDetour();
	InitDetour->Init(_Init, InitHook);
	InitDetour->Commit();
}