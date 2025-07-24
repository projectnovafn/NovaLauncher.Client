#include "UObject/UObjectGlobals.h"
#include "UObject/Class.h"

#include "KismetMemoryLibrary.h"

UObject* StaticFindObject(UClass* ObjectClass, UObject* InObjectPackage, const TCHAR* ObjectName, bool ExactClass)
{
    static auto StaticFindObject = UKismetMemoryLibrary::Get<UObject * (__fastcall*)(UClass*, UObject*, const TCHAR*, bool)>(L"StaticFindObject");

    return StaticFindObject(ObjectClass, InObjectPackage, ObjectName, ExactClass);
}

UObject* StaticLoadObject(UClass* ObjectClass, UObject* InOuter, const TCHAR* InName, const TCHAR* Filename,
    uint32 LoadFlags, UPackageMap* Sandbox, bool bAllowObjectReconciliation)
{
    static auto StaticLoadObject = UKismetMemoryLibrary::Get<UObject * (__fastcall*)(UClass*, UObject*, const TCHAR*, const TCHAR*, uint32, UPackageMap*, bool)>(L"StaticLoadObject");

    return StaticLoadObject(ObjectClass, InOuter, InName, Filename, LoadFlags, Sandbox, bAllowObjectReconciliation);
}

UObject* GetTransientPackage()
{
    static auto Transient = StaticFindObject(L"/Engine/Transient");

    return Transient;
}