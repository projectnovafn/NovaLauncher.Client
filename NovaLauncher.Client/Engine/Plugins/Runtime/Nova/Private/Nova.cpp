#include "Nova.h"
#include "Core.h"
#include "Patches.h"
#include "Mods.h"
#include "Presidio.h"

#include "Kismet/KismetSystemLibrary.h"

#include <Windows.h>
#include <sstream>

LPTOP_LEVEL_EXCEPTION_FILTER(*_SetUnhandledExceptionFilter)(LPTOP_LEVEL_EXCEPTION_FILTER) = SetUnhandledExceptionFilter;
LPTOP_LEVEL_EXCEPTION_FILTER SetUnhandledExceptionFilterHook(LPTOP_LEVEL_EXCEPTION_FILTER TopLevelExceptionFilter)
{
    auto Result = _SetUnhandledExceptionFilter(TopLevelExceptionFilter);

    if (FPlatformMisc::IsWithinImage(TopLevelExceptionFilter))
    {
        GNova->Loaded();
    }

    return Result;
}

int UNova::GetPreferredSlot(EFortWeaponType WeaponType)
{
    if (PreferredSlots.IsValidIndex((int32)WeaponType))
        return PreferredSlots[(int32)WeaponType];

    return -1;
}

void UNova::PostInit()
{
    if (UKismetSystemLibrary::GetFortniteVersion() == 9.41 || UKismetSystemLibrary::GetFortniteVersion() == 10.40)
    {
        EditingMod::Init();
        QuickBarMod::Init();
        CrashPatch::PostInit();
    }
}

void UNova::Loaded()
{    
    //GLogConsole->Show(true);

    PreferredSlots = QuickBarMod::GetPreferredSlots();

    ExceptionFilterDetour->Remove();

    CrashPatch::Init();
    UFortEnginePatches::Init();
    FCurlHttpRequestPatch::Init();

    GPresidio->Init();
}

void UNova::Init()
{
    ExceptionFilterDetour = new UDetour();
    ExceptionFilterDetour->Init(_SetUnhandledExceptionFilter, SetUnhandledExceptionFilterHook);
    ExceptionFilterDetour->Commit();

    FPlatformMisc::ResumeMainThread();
}