#include "KismetDetoursLibrary.h"
#include "vendor/memcury.h"
#include <detours.h>

static uint8_t* AllocatePageNearAddress(void* targetAddr)
{
    SYSTEM_INFO SysInfo;
    GetSystemInfo(&SysInfo);
    const uint64_t PageSize = SysInfo.dwPageSize;

    uint64_t StartAddr = (uint64_t(targetAddr) & ~(PageSize - 1));
    uint64_t MinAddr = min(StartAddr - 0x7FFFFF00, (uint64_t)SysInfo.lpMinimumApplicationAddress);
    uint64_t MaxAddr = max(StartAddr + 0x7FFFFF00, (uint64_t)SysInfo.lpMaximumApplicationAddress);

    uint64_t StartPage = (StartAddr - (StartAddr % PageSize));

    for (uint64_t PageOffset = 1; PageOffset; PageOffset++)
    {
        uint64_t ByteOffset = PageOffset * PageSize;
        uint64_t HighAddr = StartPage + ByteOffset;
        uint64_t LowAddr = (StartPage > ByteOffset) ? StartPage - ByteOffset : 0;

        bool NeedsExit = HighAddr > MaxAddr && LowAddr < MinAddr;

        if (HighAddr < MaxAddr)
        {
            void* OutAddr = VirtualAlloc((void*)HighAddr, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (OutAddr)
                return (uint8_t*)OutAddr;
        }

        if (LowAddr > MinAddr)
        {
            void* OutAddr = VirtualAlloc((void*)LowAddr, PageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (OutAddr != nullptr)
                return (uint8_t*)OutAddr;
        }

        if (NeedsExit)
        {
            break;
        }
    }

    return nullptr;
}

void UKismetDetoursLibrary::AddHookInternal(void** Target, void* Detour, EDetourType Type)
{
    if (Type == EDetourType::Detour)
    {
        DetourAttach(Target, Detour);
    }
    else if (Type == EDetourType::Rel32Call)
    {
        auto Impl = (uint8*)(*Target);
        auto NearPage = AllocatePageNearAddress(Impl);

        if (!NearPage)
        {
            return;
        }

        uint8_t Shellcode[] =
        {
            0x49, 0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // movabs rax, 0x0
            0x41, 0xFF, 0xE4,             // jmp rax
            0xC3                          // ret
        };

        if (Detour != NULL)
        {
            memcpy(Shellcode + 2, &Detour, 8);
            memcpy(NearPage, Shellcode, sizeof(Shellcode));

            auto Offset = NearPage - (Impl + 5);

            memcpy(Impl + 1, &Offset, sizeof(int));
        }
        else
        {
            memset(Impl, 0x90, sizeof(int) + 1);
        }
    }
    else if (Type == EDetourType::VFSwap)
    {
        while (true)
        {
            auto FuncRef = Memcury::Scanner::FindDataPointerRef(*Target);

            if (!FuncRef.IsValid()) break;

            *FuncRef.GetAs<void**>() = Detour;
        }
    }
}

void UKismetDetoursLibrary::RemoveHookInternal(void** Target, void* Detour, EDetourType Type)
{
    if (Type == EDetourType::Detour)
    {
        DetourDetach(Target, Detour);
    }
}

void UKismetDetoursLibrary::BeginTransaction()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
}

void UKismetDetoursLibrary::CommitTransaction()
{
    DetourTransactionCommit();
}