#include "Windows\WindowsPlatformProcess.h"
#include "Windows\WindowsRunnableThread.h"

FRunnableThread* FWindowsPlatformProcess::CreateRunnableThread()
{
	return new FRunnableThreadWin();
}

bool FWindowsPlatformProcess::SupportsMultithreading()
{
	return true;
}

FProcHandle FWindowsPlatformProcess::CreateProc(const TCHAR* URL, const TCHAR* Parms, bool bLaunchDetached, bool bLaunchHidden, bool bLaunchReallyHidden, uint32* OutProcessID, int32 PriorityModifier, const TCHAR* OptionalWorkingDirectory, void* PipeWriteChild, void* PipeReadChild, void* PipeStdErrChild)
{
	uint32 CreateFlags = NORMAL_PRIORITY_CLASS;
	if (PriorityModifier < 0)
	{
		CreateFlags = (PriorityModifier == -1) ? BELOW_NORMAL_PRIORITY_CLASS : IDLE_PRIORITY_CLASS;
	}
	else if (PriorityModifier > 0)
	{
		CreateFlags = (PriorityModifier == 1) ? ABOVE_NORMAL_PRIORITY_CLASS : HIGH_PRIORITY_CLASS;
	}

	if (bLaunchDetached)
	{
		CreateFlags |= DETACHED_PROCESS;
	}

	uint32 dwFlags = 0;
	uint16 ShowWindowFlags = SW_HIDE;
	if (bLaunchReallyHidden)
	{
		dwFlags = STARTF_USESHOWWINDOW;
	}
	else if (bLaunchHidden)
	{
		dwFlags = STARTF_USESHOWWINDOW;
		ShowWindowFlags = SW_SHOWMINNOACTIVE;
	}

	if (PipeWriteChild != nullptr || PipeReadChild != nullptr || PipeStdErrChild != nullptr)
	{
		dwFlags |= STARTF_USESTDHANDLES;
	}

	STARTUPINFO StartupInfo = {
		sizeof(STARTUPINFO),
		NULL, NULL, NULL,
		(::DWORD)CW_USEDEFAULT,
		(::DWORD)CW_USEDEFAULT,
		(::DWORD)CW_USEDEFAULT,
		(::DWORD)CW_USEDEFAULT,
		(::DWORD)0, (::DWORD)0, (::DWORD)0,
		(::DWORD)dwFlags,
		ShowWindowFlags,
		0, NULL,
		HANDLE(PipeReadChild),
		HANDLE(PipeWriteChild),
		HANDLE(PipeStdErrChild)
	};

	bool bInheritHandles = (dwFlags & STARTF_USESTDHANDLES) != 0;

	PROCESS_INFORMATION ProcInfo;

	if (!CreateProcess(URL, (LPWSTR)Parms, nullptr, nullptr, bInheritHandles, (::DWORD)CreateFlags, NULL, OptionalWorkingDirectory, &StartupInfo, &ProcInfo))
	{
		MessageBoxA(0, "CreatePRoc Failed", 0, 0);

		return FProcHandle();
	}

	if (OutProcessID != nullptr)
	{
		*OutProcessID = ProcInfo.dwProcessId;
	}

	::CloseHandle(ProcInfo.hThread);

	return FProcHandle(ProcInfo.hProcess);
}