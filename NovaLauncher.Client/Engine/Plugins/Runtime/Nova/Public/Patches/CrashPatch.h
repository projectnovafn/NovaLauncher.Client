#pragma once

class CrashPatch
{
private:

	static void CheckImageIntegrityHook();
	static void CheckImageIntegrityAtRuntimeHook();

public:

	static void PostInit();
	static void Init();
};