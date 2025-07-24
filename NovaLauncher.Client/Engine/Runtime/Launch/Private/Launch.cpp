#include "Core.h"
#include "Nova.h"
#include "Presidio.h"

int32 GuardedMain(const TCHAR* CmdLine)
{
	GNova = new UNova();
	GPresidio = new UPresidio();

	GNova->Init();

	return 0;
}