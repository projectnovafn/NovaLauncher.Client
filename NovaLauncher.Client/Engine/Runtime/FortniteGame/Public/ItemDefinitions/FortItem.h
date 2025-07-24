#pragma once
#include "CoreUObject.h"
#include "Engine/DataAsset.h"
#include "ItemDefinitions/FortItem.h"
#include "GameplayTagContainer.h"

class UFortWorldItem;

enum class EFortWeaponType : uint8 // <- Custom
{
	None = 0,
	RangedAny = 1,
	Pistol = 2,
	Shotgun = 3,
	Rifle = 4,
	SMG = 5,
	Sniper = 6,
	Utility = 7,
	Consumable = 8
};

class UFortItemDefinition : public UPrimaryDataAsset
{
	GENERATED_UCLASS_BODY(UFortItemDefinition, FortniteGame)
public:

	UPROPERTY_BOOL(bAllowMultipleStacks)
	bool bAllowMultipleStacks;

	UPROPERTY(int, MaxStackSize)
	int MaxStackSize;

	UPROPERTY(uint8, Rarity)
	uint8 Rarity;

	UPROPERTY(FGameplayTagContainer, GameplayTags)
	FGameplayTagContainer GameplayTags;

	EFortWeaponType GetWeaponType();
};

class UFortItem : public UObject
{
	GENERATED_UCLASS_BODY(UFortItem, FortniteGame)
public:
};