// Shoot Them Up game. All Rights Reserved.


#include "Weapon/Components/STUAIWeaponComponent.h"
#include "Weapon/STUWeapon.h"

void USTUAIWeaponComponent::StartFire()
{
	if(!CanFire()) return;

	if(CurrentWeapon->IsAmmoEmpty()) 
	{
		SwitchWeapon();
	}else
	{
		CurrentWeapon->StartFire();
	}
}

void USTUAIWeaponComponent::SwitchWeapon()
{
	if(!CanEquip()) return;

	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while(NextIndex != CurrentWeaponIndex)
	{
		if(!Weapons[NextIndex]->IsAmmoEmpty()) break;
		NextIndex = (NextIndex + 1) % Weapons.Num();
	}

	if(CurrentWeaponIndex != NextIndex)
	{
		CurrentWeaponIndex = NextIndex;
		EquipWeapon(CurrentWeaponIndex);
	}
}