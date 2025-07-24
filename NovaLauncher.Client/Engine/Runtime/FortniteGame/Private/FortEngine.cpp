#include "FortEngine.h"
#include "Nova.h"
#include "Logging/LogMacros.h"

void UFortEngine::Init(void* EngineLoop)
{
	GEngine = this;
	
	GNova->PostInit();
}