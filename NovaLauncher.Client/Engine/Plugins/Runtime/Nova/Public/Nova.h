#pragma once

#include "KismetDetoursLibrary.h"
#include "KismetMemoryLibrary.h"

#include "ItemDefinitions/FortItem.h"

class UEngine;

class UNova
{
private:

	UDetour* ExceptionFilterDetour = NULL;

	TArray<int32> PreferredSlots;

public:
	int GetPreferredSlot(EFortWeaponType WeaponType);

	void PostInit();

	void Init();
	void Loaded();
};