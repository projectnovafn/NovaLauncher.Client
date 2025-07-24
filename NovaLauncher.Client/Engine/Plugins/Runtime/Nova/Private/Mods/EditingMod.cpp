#include "Mods/EditingMod.h"
#include "Engine/Engine.h"
#include "Athena/Player/FortPlayerControllerAthena.h"
#include "BuildingPlayerPrimitivePreview.h"

#include "Logging/LogMacros.h"

#include <windows.h>

UDetour* EditingDetour = NULL;
UDetour* EorDetour = NULL;
UDetour* InstantResetDetour = NULL;

void EditingMod::PerformBuildingEditInteractionHook(uintptr_t a1)
{
    if (!GEngine->GameInstance || !GEngine->GameInstance->LocalPlayers[0])
        return;

    auto PlayerController = GEngine->GameInstance->LocalPlayers[0]->PlayerController->As<AFortPlayerControllerAthena>();
    if (!PlayerController->TargetedBuilding || 
        PlayerController->TargetedBuilding->IsA<ABuildingPlayerPrimitivePreview>())
        return;

    return PerformBuildingEditInteraction(a1);
}

void EditingMod::OnEditPreviewReleaseHook(uintptr_t Controller)
{
    if (!Controller) return;

    CompleteEdit(Controller);
    OnEditPreviewRelease(Controller);
}

void EditingMod::OnEditResetHook(uintptr_t Controller)
{
    if (!Controller) return;

    OnEditReset(Controller);
    CompleteEdit(Controller);
}

void EditingMod::Init()
{
    FString CommandLine(GetCommandLineW());

    PerformBuildingEditInteraction = UKismetMemoryLibrary::Get<decltype(PerformBuildingEditInteraction)>(L"PerformBuildingEditInteraction");
    CompleteEdit = UKismetMemoryLibrary::Get<decltype(CompleteEdit)>(L"CompleteEdit");
    OnEditPreviewRelease = UKismetMemoryLibrary::Get<decltype(OnEditPreviewRelease)>(L"OnEditPreviewRelease");
    OnEditReset = UKismetMemoryLibrary::Get<decltype(OnEditReset)>(L"OnEditReset");

    EditingDetour = new UDetour();
    EorDetour = new UDetour();
    InstantResetDetour = new UDetour();

    EditingDetour->Init(PerformBuildingEditInteraction, PerformBuildingEditInteractionHook);
    EorDetour->Init(OnEditPreviewRelease, OnEditPreviewReleaseHook);
    InstantResetDetour->Init(OnEditReset, OnEditResetHook);

    if (CommandLine.Find(L"-preedit") != INDEX_NONE)
        EditingDetour->Commit();

    if (CommandLine.Find(L"-eor") != INDEX_NONE)
        EorDetour->Commit();

    if (CommandLine.Find(L"-instantreset") != INDEX_NONE)
        InstantResetDetour->Commit();
}