#pragma once
#include <cstdint>

class CameraUpdatePatch
{
private:
	static inline void (*ServerUpdateCamera)(uintptr_t, __int64, int);
	static inline float (*GetTimeSeconds)(uintptr_t);

	static void ServerUpdateCameraHook(uintptr_t, __int64, int);

public:
	static void Init();
};