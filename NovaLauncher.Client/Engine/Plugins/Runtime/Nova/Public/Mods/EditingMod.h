#pragma once
#include <cstdint>
#include "KismetDetoursLibrary.h"

class EditingMod
{
private:
	static inline void (*PerformBuildingEditInteraction)(uintptr_t);
	static inline void* (*CompleteEdit)(uintptr_t);
	static inline void* (*OnEditPreviewRelease)(uintptr_t);
	static inline void* (*OnEditReset)(uintptr_t);

	static void PerformBuildingEditInteractionHook(uintptr_t);
	static void OnEditPreviewReleaseHook(uintptr_t);
	static void OnEditResetHook(uintptr_t);

public:
	static void Init();
};