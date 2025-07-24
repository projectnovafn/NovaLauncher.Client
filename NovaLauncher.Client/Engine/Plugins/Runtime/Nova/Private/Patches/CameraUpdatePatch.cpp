#include "Patches/CameraUpdatePatch.h"
#include "KismetDetoursLibrary.h"

UDetour* CameraUpdateDetour = NULL;

void CameraUpdatePatch::ServerUpdateCameraHook(uintptr_t CameraManager, __int64 a2, int a3)
{
    static float LastTimeSeconds = 0.0;
    auto TimeSeconds = GetTimeSeconds(CameraManager);

    float TimeDifference = TimeSeconds - LastTimeSeconds;
    float MinExecutionRate = 0.066f;

    if (TimeDifference >= MinExecutionRate)
    {
        LastTimeSeconds = TimeSeconds;
        return ServerUpdateCamera(CameraManager, a2, a3);
    }
}

void CameraUpdatePatch::Init()
{
    ServerUpdateCamera = UKismetMemoryLibrary::Get<decltype(ServerUpdateCamera)>(L"ServerUpdateCamera");
    GetTimeSeconds = UKismetMemoryLibrary::Get<decltype(GetTimeSeconds)>(L"GetTimeSeconds");

    CameraUpdateDetour = new UDetour();
    CameraUpdateDetour->Init(ServerUpdateCamera, ServerUpdateCameraHook);
    CameraUpdateDetour->Commit();
}
