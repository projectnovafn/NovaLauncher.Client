#include "Presidio.h"
#include "Core.h"

#include <Windows.h>
#include <shlobj.h>

std::wstring GetCurrentWorkingDirectory()
{
    TCHAR Path[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, Path);
	return Path;
}

bool UPresidio::IsActive()
{
	FString CommandLine(GetCommandLineW());

	return CommandLine.Find(L"-p=") != INDEX_NONE;
}

void UPresidio::Init()
{
	if (!this->IsActive())
		return;

	auto LibraryPath = GetCurrentWorkingDirectory() + L"\\NovaLauncher\\Assets\\Presidio\\Presidio.dll";

	if (!LibraryPath.empty())
		LoadLibraryW(LibraryPath.c_str());
}