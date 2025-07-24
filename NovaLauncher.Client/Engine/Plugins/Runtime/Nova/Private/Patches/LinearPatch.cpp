#include "Patches/LinearPatch.h"
#include "KismetDetoursLibrary.h"

#include "Windows.h"

static bool (*_ShowNewControllerSenseOptions)(__int64);
UDetour* ShowNewControllerSenseOptionsDetour = NULL;

bool LinearPatch::ShowNewControllerSenseOptionsHook(__int64 a1)
{
	bool result = _ShowNewControllerSenseOptions(a1);
	UE_LOG(LogTest, VeryVerbose, TEXT("Result: %d"), result);

	return true;
}

void LinearPatch::Init()
{
    _ShowNewControllerSenseOptions = UKismetMemoryLibrary::Get<decltype(_ShowNewControllerSenseOptions)>(L"ShowNewControllerSenseOptions");

    ShowNewControllerSenseOptionsDetour = new UDetour();
    ShowNewControllerSenseOptionsDetour->Init(_ShowNewControllerSenseOptions, ShowNewControllerSenseOptionsHook);

    FString CommandLine(GetCommandLineW());
    if (CommandLine.Find(L"-linear") != INDEX_NONE)
        ShowNewControllerSenseOptionsDetour->Commit();
}
