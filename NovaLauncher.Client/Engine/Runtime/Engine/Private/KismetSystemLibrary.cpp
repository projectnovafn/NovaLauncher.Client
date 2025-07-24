#include "Kismet/KismetSystemLibrary.h"

FString UKismetSystemLibrary::GetEngineVersion()
{
	static auto GetEngineVersion = UKismetMemoryLibrary::Get<FString * (*)(FString*)>(L"UKismetSystemLibrary::GetEngineVersion");

	FString Temp;
	return *GetEngineVersion(&Temp);
}