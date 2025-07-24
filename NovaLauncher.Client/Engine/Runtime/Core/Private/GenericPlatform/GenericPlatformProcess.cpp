#pragma once

#include "GenericPlatform\GenericPlatformProcess.h"

FRunnableThread* FGenericPlatformProcess::CreateRunnableThread()
{
	return nullptr;
}

bool FGenericPlatformProcess::SupportsMultithreading()
{
	return false;
}