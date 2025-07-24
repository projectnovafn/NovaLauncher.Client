#include "KismetMemoryLibrary.h"
#include "UObject/Class.h"

#include "vendor/memcury.h"

template <typename T>
static uint32 FindOffset(void* Object, T Target, int Start = 0)
{
    auto Ptr = (uint8*)Object;

    for (int i = Start; i < 0x2000; i++)
    {
        if (*(T*)(Ptr + i) == Target)
            return i;
    }

    return -1;
}

static std::map<std::wstring, std::function<void* ()>>& GetFunctionMap()
{
    static std::map<std::wstring, std::function<void* ()>> FunctionMap;
    return FunctionMap;
}

void* UKismetMemoryLibrary::GetInternal(const TCHAR* Name)
{
    auto& FunctionMap = GetFunctionMap();
    auto it = FunctionMap.find(Name);
    if (it != FunctionMap.end())
    {
        return it->second();
    }

    return NULL;
}

void UKismetMemoryLibrary::AddInternal(const TCHAR* Name, FunctionType Function)
{
    GetFunctionMap()[Name] = Function;
}

static std::map<std::wstring, std::function<uint32()>>& GetOffsetFunctionMap()
{
    static std::map<std::wstring, std::function<uint32()>> OffsetFunctionMap;
    return OffsetFunctionMap;
}

uint32 UKismetMemoryLibrary::GetOffsetInternal(const TCHAR* Name)
{
    static std::map<std::wstring, uint32> CachedOffsets;

    auto& FunctionMap = GetOffsetFunctionMap();
    auto it = CachedOffsets.find(Name);
    if (it != CachedOffsets.end())
    {
        return it->second;
    }

    auto funcIt = FunctionMap.find(Name);
    if (funcIt != FunctionMap.end())
    {
        uint32 result = funcIt->second();
        CachedOffsets[Name] = result;
        return result;
    }

    return -1;
}

void UKismetMemoryLibrary::AddOffsetInternal(const TCHAR* Name, OffsetFunctionType Function)
{
    GetOffsetFunctionMap()[Name] = Function;
}

static double GetEngineVersion()
{
    auto GetEngineVersion = Memcury::Scanner::FindPattern("40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 8B C8 41 B8 04 ? ? ? 48 8B D3")
        .GetAs<FString * (*)(FString*)>();

    FString Result;
    auto EngineVersion = GetEngineVersion(&Result)->ToString();
    return std::stod(EngineVersion.substr(0, 4));
}
	
DEFINE_MEMORY_VARIABLE(UKismetSystemLibrary_GetEngineVersion, "UKismetSystemLibrary::GetEngineVersion")
{
    return Memcury::Scanner::FindPattern("40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 8B C8 41 B8 04 ? ? ? 48 8B D3")
        .GetAs<FString * (*)(FString*)>();
}

DEFINE_MEMORY_VARIABLE(FMemory_Realloc, "FMemory::Realloc")
{
    return Memcury::Scanner::FindStringRef(L"AbilitySystem.Debug.NextTarget", 0)
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FName_ToString, "FName::ToString")
{
    return Memcury::Scanner::FindStringRef(L"Destroying blueprint contexts for Local Player ' %s ' of Game Instance ' %s '")
        .ScanFor({ 0xE8 }, false, 1)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FName_FName, "FName::FName")
{
    return Memcury::Scanner::FindStringRef(L"Unrecognized property %s on class %s")
        .ScanFor({ 0x48, 0x8B, 0xCB })
        .ScanFor({ 0xE8 }, false)
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UObject_ProcessEvent, "UObject::ProcessEvent")
{
    if (auto Addr = Memcury::Scanner::FindPatterns({ "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 8B 41 0C 45 33 F6 3B 05", "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8D 6C 24 ? 48 89 9D ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C5 48 89 85 ? ? ? ? 4D 8B F8" }, false).GetAs<void*>())
        return Addr;

    if (auto Addr = Memcury::Scanner::FindStringRef(L"UMeshNetworkComponent::ProcessEvent: Invalid mesh network node type: %s", 0, false)
        .ScanFor({ 0x5F })
        .ScanFor({ 0xE9 })
        .RelativeOffset(1)
        .GetAs<void*>())
        return Addr;

    return Memcury::Scanner::FindStringRef(L"AccessNoneNoContext")
        .ScanFor({ 0x40, 0x55 })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FWindowsPlatformMisc_RequestExit, "FWindowsPlatformMisc::RequestExit")
{
    return Memcury::Scanner::FindStringRef(L"FPlatformMisc::RequestExit(%i)", 0)
        .ScanFor({ 0x40, 0x53 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FWindowsPlatformMisc_RequestExitWithStatus, "FWindowsPlatformMisc::RequestExitWithStatus")
{
    return Memcury::Scanner::FindStringRef(L"FPlatformMisc::RequestExitWithStatus(%i, %d) - return code will be ignored by the generic implementation.", 0)
        .ScanFor({ 0x40, 0x53 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(FCurlHttpRequest_ProcessRequest, "FCurlHttpRequest::ProcessRequest")
{
    auto ProcessRequest = Memcury::Scanner::FindPattern("40 53 55 57 41 54 48 81 EC ? ? ? ? 48 8B 99");
    if (ProcessRequest.IsValid())
        return ProcessRequest.GetAs<void*>();

    auto StringRef = Memcury::Scanner::FindStringRef(L"STAT_FCurlHttpRequest_ProcessRequest");
    if (StringRef.IsValid())
    {
        return StringRef
            .ScanFor({ 0x40, 0x53 }, false)
            .GetAs<void*>();
    }

    StringRef = Memcury::Scanner::FindStringRef(L"Could not set libcurl options for easy handle, processing HTTP request failed. Increase verbosity for additional information.");
    if (StringRef.IsValid())
    {
        return StringRef
            .ScanFor({ 0x48, 0x81, 0xEC }, false)
            .ScanFor({ 0x40 }, false)
            .GetAs<void*>();
    }

    return NULL;
}

DEFINE_MEMORY_VARIABLE(CheckImageIntegrity, "CheckImageIntegrity")
{
    return Memcury::Scanner::FindPatterns({ "48 83 EC 28 48 8D 4C 24 ? FF 15 ? ? ? ? F2 0F 10 05", "48 83 EC 38 48 8D 4C 24 ? 0F 29 74 24" })
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(CheckImageIntegrityAtRuntime, "CheckImageIntegrityAtRuntime")
{
    return Memcury::Scanner::FindStringRef(L"HITCHHUNTER: Hitch in CIIAR: %dms")
        .ScanFor({ 0x48, 0x83 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(Broadcast, "Broadcast")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 8D 8D ? ? ? ? E8 ? ? ? ? F3 0F 10 0D ? ? ? ? 48 8D 8D ? ? ? ? 4C 8B C0 E8 ? ? ? ? E8 ? ? ? ? 84 C0 0F 84", false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(IntegrityChecksumCheck, "IntegrityChecksumCheck")
{
    return Memcury::Scanner::FindPattern("8B 05 ? ? ? ? 33 05 ? ? ? ? 01 05 ? ? ? ? E9")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UACTick, "UACTick")
{
    auto UACClientTick = Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC 30 48 8B 01 4C 8B C2 48 8D 54 24 ? 48 8B D9 FF 50 30");
    if (!UACClientTick.IsValid())
    {
        UACClientTick = Memcury::Scanner::FindPattern("4C 8B DC 55 57 41 56 49 8D AB 58 FD FF FF");
    }

    return UACClientTick
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(PerformBuildingEditInteraction, "PerformBuildingEditInteraction")
{
    if (GetEngineVersion() == 4.21)
        return NULL;

    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B F9 E8 ? ? ? ? 48 8D 97")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(OnEditPreviewRelease, "OnEditPreviewRelease")
{
    if (GetEngineVersion() == 4.21)
        return NULL;

    return Memcury::Scanner::FindStringRef(L"[AFortPlayerControllerAthena::OnPickingTouch_End()] StartPosition [%s] Direction [%s]")
        .ScanFor({ 0x32, 0xDB })
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(CompleteEdit, "CompleteEdit")
{
    if (GetEngineVersion() == 4.21)
        return NULL;

    auto Addr = Memcury::Scanner::FindPattern("E9 ? ? ? ? 48 8B 03 48 83 C4 20")
        .RelativeOffset(1)
        .GetAs<void*>();

    if (Addr)
        return Addr;

    return Memcury::Scanner::FindStringRef("CompleteBuildingEditInteraction")
        .ScanFor({ 0x48, 0x8D }, false)
        .RelativeOffset(3)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(OnEditReset, "OnEditReset")
{
    if (GetEngineVersion() == 4.21)
        return NULL;

    auto addr = Memcury::Scanner::FindPattern("48 8B 89 ? ? ? ? 48 85 C9 74 0A 48 8B 01 48 FF A0 A8 0B 00 00")
        .GetAs<void*>();

    if (!addr)
        addr = Memcury::Scanner::FindPattern("48 8B 89 ? ? ? ? 48 85 C9 74 0A 48 8B 01 48 FF A0 88 0B 00 00")
        .GetAs<void*>();

    if (!addr)
        addr = Memcury::Scanner::FindPattern("48 8B 89 ? ? ? ? 48 85 C9 74 0A 48 8B 01 48 FF A0 A0 0B 00 00")
        .GetAs<void*>();

    if (!addr)
    {
        addr = Memcury::Scanner::FindStringRef(L"[AFortPlayerControllerAthena::OnPickingTouch_End()] CurrentPickingType [EAthenaPickerType::EditMode], RetResult [%s]")
            .ScanFor({ 0x41, 0x88 }, false)
            .ScanFor({ 0xE8 }, false)
            .RelativeOffset(1)
            .GetAs<void*>();
    }

    return addr;
}

DEFINE_MEMORY_VARIABLE(AFortQuickBars_AddItemInternal, "AFortQuickBars::AddItemInternal")
{
    if (GetEngineVersion() != 4.23)
        return NULL;

    return Memcury::Scanner::FindStringRef(L"Attempting to add harvesting tool %s to quickbar %d in slot %d.")
        .ScanFor({ 0x44, 0x89, 0x4C }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(AFortQuickBars_SwapItems, "AFortQuickBars::SwapItems")
{
    return Memcury::Scanner::FindStringRef(L"Bad call to AFortQuickBars::SwapItems with index %d %d!")
        .ScanFor({ 0x44, 0x88 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(GetTimeSeconds, "GetTimeSeconds")
{
    return Memcury::Scanner::FindPattern("E8 ? ? ? ? 48 83 BF ? ? ? ? ? 0F 28 F0")
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(ServerUpdateCamera, "ServerUpdateCamera")
{
    return Memcury::Scanner::FindStringRef(L"STAT_ServerUpdateCamera")
        .ScanFor({ 0xF2, 0x0F, 0x11 })
        .ScanFor({ 0xE8 })
        .RelativeOffset(1)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortEngine_Init, "UFortEngine::Init")
{
    return Memcury::Scanner::FindPattern("48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 8D 15 ? ? ? ? 41 B8")
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UOnlineAccountCommon_ForceLogout, "UOnlineAccountCommon::ForceLogout")
{
    return Memcury::Scanner::FindStringRef(L"[UOnlineAccountCommon::ForceLogout] ForceLogout (local=%s) requested with reason \"%s\"")
        .ScanFor({ 0x44, 0x88 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UOnlineAccountCommon_ContinueLoggingOut, "UOnlineAccountCommon::ContinueLoggingOut")
{
    return Memcury::Scanner::FindStringRef(L"[UOnlineAccountCommon::ContinueLoggingOut] Aborting ContinueLoggingOut due to unexpected state change [%s] -> [%s]. This is fatal.")
        .ScanFor({ 0x40, 0x53 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_VARIABLE(UFortOnlineAccount_ForceLogout, "UFortOnlineAccount::ForceLogout")
{
    return Memcury::Scanner::FindStringRef(L"[UFortOnlineAccount::ForceLogout] ForceLogout requested. Local=%s LogoutReason=[%s]")
        .ScanFor({ 0x44, 0x88 }, false)
        .GetAs<void*>();
}

DEFINE_MEMORY_OFFSET(UField_Next, "UField::Next")
{
    return 0x28;
}

DEFINE_MEMORY_OFFSET(UStruct_SuperStruct, "UStruct::SuperStruct")
{
    auto Class = FindObject<UClass>(L"/Script/Engine.Actor");
    auto SuperStruct = FindObject<UClass>(L"/Script/CoreUObject.Object");

    return FindOffset(Class, SuperStruct);
}

DEFINE_MEMORY_OFFSET(UStruct_Children, "UStruct::Children")
{
    if (GetEngineVersion() <= 4.21)
        return 0x38;

    auto Class = FindObject<UClass>(L"/Script/Engine.Actor");
    auto Function = FindObject<UFunction>(L"/Script/Engine.Actor.WasRecentlyRendered");
    auto Offset = FindOffset(Class, Function);

    if (Offset == -1)
    {
        return 0x48;
    }

    return Offset;
}

DEFINE_MEMORY_OFFSET(UStruct_PropertiesSize, "UStruct::PropertiesSize")
{
    auto Struct = FindObject<UStruct>(L"/Script/CoreUObject.Transform");
    auto Offset = FindOffset(Struct, sizeof(FTransform));

    if (Offset == -1)
    {
        return UKismetMemoryLibrary::Get(L"UStruct::Children") + 8;
    }

    return Offset;
}

DEFINE_MEMORY_OFFSET(UClass_ClassDefaultObject, "UClass::ClassDefaultObject")
{
    auto Class = FindObject<UClass>(L"/Script/Engine.Actor");
    auto DefaultObject = FindObject<UObject>(L"/Script/Engine.Default__Actor");

    return FindOffset(Class, DefaultObject);
}

DEFINE_MEMORY_OFFSET(UProperty_Offset_Internal, "UProperty::Offset_Internal")
{
    return 0x44;
}

DEFINE_MEMORY_OFFSET(UBoolProperty_ByteOffset, "UBoolProperty::ByteOffset")
{
    return 0x71;
}

DEFINE_MEMORY_OFFSET(UBoolProperty_ByteMask, "UBoolProperty::ByteMask")
{
    return 0x72;
}

DEFINE_MEMORY_OFFSET(UBoolProperty_FieldMask, "UBoolProperty::FieldMask")
{
    return 0x73;
}

DEFINE_MEMORY_OFFSET(UFunction_Func, "UFunction::Func")
{
    if (GetEngineVersion() <= 4.21)
        return 0xB0;

    if (GetEngineVersion() >= 4.25)
        return 0xF0;

    return 0xC0;
}

DEFINE_MEMORY_OFFSET(UFunction_ParmsSize, "UFunction::ParmsSize")
{
    auto Function = FindObject<UFunction>(L"/Script/Engine.DecalComponent.SetFadeOut");

    return FindOffset<uint16>(Function, 0x9);
}

DEFINE_MEMORY_OFFSET(FFrame_PropertyChainForCompiledIn, "FFrame::PropertyChainForCompiledIn")
{
    return 0x80;
}

DEFINE_MEMORY_OFFSET(UStruct_ChildProperties, "UStruct::ChildProperties")
{
    return 0x50;
}

DEFINE_MEMORY_OFFSET(FProperty_Offset_Internal, "FProperty::Offset_Internal")
{
    return 0x4C;
}

DEFINE_MEMORY_OFFSET(FField_Next, "FField::Next")
{
    return 0x20;
}

DEFINE_MEMORY_OFFSET(FField_Name, "FField::Name")
{
    return 0x28;
}

DEFINE_MEMORY_OFFSET(FStructProperty_Struct, "FStructProperty::Struct")
{
    return 0x78;
}

DEFINE_MEMORY_OFFSET(FBoolProperty_ByteOffset, "FBoolProperty::ByteOffset")
{
    return 0x79;
}

DEFINE_MEMORY_OFFSET(FBoolProperty_ByteMask, "FBoolProperty::ByteMask")
{
    return 0x7A;
}

DEFINE_MEMORY_OFFSET(FBoolProperty_FieldMask, "FBoolProperty::FieldMask")
{
    return 0x7B;
}

DEFINE_MEMORY_VARIABLE(StaticFindObject, "StaticFindObject")
{
    return Memcury::Scanner::FindStringRef(L"Illegal call to StaticFindObject() while serializing object data!", 0, false)
        .ScanFor({ 0x48, 0x89, 0x5C }, false)
        .GetAs<void*>();
}