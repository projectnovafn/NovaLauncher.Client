#include "Mods/QuickBarMod.h"
#include "KismetDetoursLibrary.h"

#include <Windows.h>
#include <sstream>
#include <string>

static void (*_AddItemInternal)(AFortQuickBars*, FGuid, EFortQuickBars, int);

UDetour* AddItemInternalDetour = NULL;

void QuickBarMod::AddItemInternalHook(AFortQuickBars* QuickBars, FGuid ItemGuid, EFortQuickBars QuickBarType, int SlotIndex)
{
	QuickBars->AddItemInternal(ItemGuid, QuickBarType, SlotIndex, _AddItemInternal);

	_AddItemInternal(QuickBars, ItemGuid, QuickBarType, SlotIndex);
}

TArray<int> QuickBarMod::GetPreferredSlots()
{
    FString CommandLine(GetCommandLineW());

    auto StartIndex = CommandLine.Find(TEXT("-preferreditems="));
    if (StartIndex == INDEX_NONE)
    {
        return {};
    }

    StartIndex += FString(TEXT("-preferreditems=")).Len();

    auto EndIndex = CommandLine.Find(TEXT("]"), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
    auto PreferredItemsString = CommandLine.Mid(StartIndex, EndIndex - StartIndex + 1);

    std::wstring Input(*PreferredItemsString);

    TArray<int> Result;
    std::wstringstream ss(Input);
    std::wstring Temp;

    if (Input[0] == L'[' && Input[Input.length() - 1] == L']')
    {
        ss.ignore();
        std::getline(ss, Temp, L']');
    }

    std::wstringstream NumStream(Temp);
    while (std::getline(NumStream, Temp, L','))
    {
        Temp.erase(0, Temp.find_first_not_of(L' '));
        Result.Add(std::stoi(Temp));
    }

    return Result;
}

void QuickBarMod::Init()
{
	_AddItemInternal = UKismetMemoryLibrary::Get<decltype(_AddItemInternal)>(L"AFortQuickBars::AddItemInternal");

	AddItemInternalDetour = new UDetour();
	AddItemInternalDetour->Init(_AddItemInternal, AddItemInternalHook);
	AddItemInternalDetour->Commit();
}