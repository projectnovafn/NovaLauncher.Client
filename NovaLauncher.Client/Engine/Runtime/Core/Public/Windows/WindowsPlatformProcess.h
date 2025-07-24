#pragma once

#include "CoreTypes.h"
#include "GenericPlatform\GenericPlatformProcess.h"

class FRunnableThread;

struct FProcHandle : public TProcHandle<Windows::HANDLE, nullptr>
{
public:
    /** Default constructor. */
    FORCEINLINE FProcHandle()
        : TProcHandle()
    {}

    /** Initialization constructor. */
    FORCEINLINE explicit FProcHandle(HandleType Other)
        : TProcHandle(Other)
    {}
};

struct FWindowsPlatformProcess : public FGenericPlatformProcess
{
	static CORE_API class FRunnableThread* CreateRunnableThread();
	static CORE_API bool SupportsMultithreading();
    static CORE_API FProcHandle CreateProc(
        const TCHAR* URL,
        const TCHAR* Parms,
        bool bLaunchDetached,
        bool bLaunchHidden,
        bool bLaunchReallyHidden,
        uint32* OutProcessID,
        int32 PriorityModifier,
        const TCHAR* OptionalWorkingDirectory,
        void* PipeWriteChild,
        void* PipeReadChild,
        void* PipeStdErrChild
    );
};

typedef FWindowsPlatformProcess FPlatformProcess;