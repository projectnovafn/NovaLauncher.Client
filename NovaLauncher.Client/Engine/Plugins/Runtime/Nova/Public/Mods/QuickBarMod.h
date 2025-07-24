#pragma once
#include "KismetMemoryLibrary.h"
#include "Inventory/FortQuickBars.h"

class QuickBarMod
{
private:
	static void AddItemInternalHook(AFortQuickBars*, FGuid, EFortQuickBars, int);

public:

	static TArray<int> GetPreferredSlots();

	static void Init();
};