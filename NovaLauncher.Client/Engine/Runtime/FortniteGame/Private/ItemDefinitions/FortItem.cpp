#include "ItemDefinitions/FortItem.h"

EFortWeaponType UFortItemDefinition::GetWeaponType()
{
    for (auto& GameplayTag : GameplayTags.GameplayTags)
    {
        auto TagName = GameplayTag.TagName.ToString();

        if (TagName.Contains(L"Item.Weapon.Ranged.Shotgun"))
            return EFortWeaponType::Shotgun;

        if (TagName.Contains(L"Weapon.Ranged.Assault"))
            return EFortWeaponType::Rifle;

        if (TagName.Contains(L"Item.Weapon.Ranged.SMG"))
            return EFortWeaponType::SMG;

        if (TagName.Contains(L"Weapon.Ranged.Pistol"))
            return EFortWeaponType::Pistol;

        if (TagName.Contains(L"Weapon.Ranged.Sniper"))
            return EFortWeaponType::Sniper;

        if (TagName.Contains(L"Athena.Item.Utility"))
            return EFortWeaponType::Utility;

        if (TagName.Contains(L"Item.Consumable"))
            return EFortWeaponType::Consumable;
    }

    return EFortWeaponType::None;
}