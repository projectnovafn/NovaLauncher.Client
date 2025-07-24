#pragma once

#include "CoreMinimal.h"

template< typename T, T InvalidHandleValue >
struct TProcHandle
{
	typedef T HandleType;
public:

	/** Default constructor. */
	FORCEINLINE TProcHandle()
		: Handle(InvalidHandleValue)
	{ }

	/** Initialization constructor. */
	FORCEINLINE explicit TProcHandle(T Other)
		: Handle(Other)
	{ }

	/** Accessors. */
	FORCEINLINE T Get() const
	{
		return Handle;
	}

	FORCEINLINE void Reset()
	{
		Handle = InvalidHandleValue;
	}

	FORCEINLINE bool IsValid() const
	{
		return Handle != InvalidHandleValue;
	}

protected:
	T Handle;
};

struct FProcHandle;

struct FGenericPlatformProcess
{
	static CORE_API class FRunnableThread* CreateRunnableThread();
	static CORE_API bool SupportsMultithreading();
};

#if PLATFORM_WINDOWS
#include "Windows\WindowsPlatformProcess.h"
#endif