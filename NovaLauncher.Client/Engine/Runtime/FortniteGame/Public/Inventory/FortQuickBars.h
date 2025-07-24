#pragma once
#include "GameFramework/Actor.h"

enum class EFortQuickBars : uint8
{
	Primary = 0,
	Secondary = 1,
	Max_None = 2
};

class FQuickBarSlot
{
	GENERATED_USTRUCT_BODY(FQuickBarSlot, FortniteGame)
public:

	UPROPERTY_STRUCT(TArray<FGuid>, Items)
	TArray<FGuid> Items;

	UPROPERTY_STRUCT(bool, bEnabled)
	bool bEnabled;
};

class FQuickBar
{
	GENERATED_USTRUCT_BODY(FQuickBar, FortniteGame)
public:

	UPROPERTY_STRUCT(int, CurrentFocusedSlot)
	int CurrentFocusedSlot;

	UPROPERTY_STRUCT(int, PreviousFocusedSlot)
	int PreviousFocusedSlot;

	UPROPERTY_STRUCT(int, SecondaryFocusedSlot)
	int SecondaryFocusedSlot;

	UPROPERTY_STRUCT(TArray<FQuickBarSlot>, Slots)
	TArray<FQuickBarSlot> Slots;

	int GetNextSlot()
	{
		for (int i = 0; i < Slots.Num(); i++)
		{
			if (!Slots[i].Items.Num())
				return i;
		}

		return -1;
	}
};

class AFortQuickBars : public AActor // <- AFortClientOnlyActor 
{
	GENERATED_UCLASS_BODY(AFortQuickBars, FortniteGame)
public:

	UPROPERTY(FQuickBar, PrimaryQuickBar)
	FQuickBar PrimaryQuickBar;

	typedef void (*AddItemInternalOriginal)(AFortQuickBars*, FGuid, EFortQuickBars, int);

	void AddItemInternal(FGuid ItemGuid, EFortQuickBars QuickBarType, int& SlotIndex, AddItemInternalOriginal Original);
};