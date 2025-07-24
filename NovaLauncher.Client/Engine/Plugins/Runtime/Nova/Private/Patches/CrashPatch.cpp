#pragma once
#include "Patches/CrashPatch.h"
#include "KismetDetoursLibrary.h"

#include "Logging/LogMacros.h"
#include "Kismet/KismetSystemLibrary.h"

#include <Windows.h>

const uint8 IntegrityPatchBytes[] = { 0x90, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0xE9 };
const uint8 BroadcastPatchBytes[] = { 0x90, 0x90, 0x90, 0x90, 0x90 };

void (*_CheckImageIntegrity)();
void (*_CheckImageIntegrityAtRuntime)();

UDetour* CheckImageIntegrityDetour = NULL;
UDetour* CheckImageIntegrityAtRuntimeDetour = NULL;

void CrashPatch::CheckImageIntegrityHook()
{
    UE_LOG(LogPatches, Log, TEXT("CheckImageIntegrityHook"));
}

void CrashPatch::CheckImageIntegrityAtRuntimeHook()
{
    UE_LOG(LogPatches, Log, TEXT("CheckImageIntegrityAtRuntimeHook"));
}

void CrashPatch::PostInit()
{
    if (UKismetSystemLibrary::GetFortniteVersion() == 10.40)
    {
        *UKismetMemoryLibrary::Get<uint8*>(L"UOnlineAccountCommon::ForceLogout") = 0xC3;
        *UKismetMemoryLibrary::Get<uint8*>(L"UOnlineAccountCommon::ContinueLoggingOut") = 0xC3;
        *UKismetMemoryLibrary::Get<uint8*>(L"UFortOnlineAccount::ForceLogout") = 0xC3;
    }
}

void CrashPatch::Init()
{
    _CheckImageIntegrity = UKismetMemoryLibrary::Get<decltype(_CheckImageIntegrity)>(L"CheckImageIntegrity");
    _CheckImageIntegrityAtRuntime = UKismetMemoryLibrary::Get<decltype(_CheckImageIntegrityAtRuntime)>(L"CheckImageIntegrityAtRuntime");

    while (true)
    {
        auto IntegrityPatch = UKismetMemoryLibrary::Get<uint8*>(L"IntegrityChecksumCheck");

        if (IntegrityPatch == NULL)
            break;

        UE_LOG(LogPatches, Log, TEXT("IntegrityPatch: %p"), IntegrityPatch);

        FMemory::Memcpy(IntegrityPatch, IntegrityPatchBytes, sizeof(IntegrityPatchBytes));
    }

    CheckImageIntegrityDetour = new UDetour();
    CheckImageIntegrityDetour->Init(_CheckImageIntegrity, CheckImageIntegrityHook);
    CheckImageIntegrityDetour->Commit();

    CheckImageIntegrityAtRuntimeDetour = new UDetour();
    CheckImageIntegrityAtRuntimeDetour->Init(_CheckImageIntegrityAtRuntime, CheckImageIntegrityAtRuntimeHook);
    CheckImageIntegrityAtRuntimeDetour->Commit();

    if (auto Broadcast = UKismetMemoryLibrary::Get<uint8*>(L"Broadcast"))
    {
        UE_LOG(LogPatches, Log, TEXT("Broadcast Patch!"));

        FMemory::Memcpy(Broadcast, BroadcastPatchBytes, sizeof(BroadcastPatchBytes));
    }
    else
    {
        UE_LOG(LogPatches, Log, TEXT("Broadcast Patch Failed!"));
    }

    *UKismetMemoryLibrary::Get<uint8*>(L"UACTick") = 0xC3;
}