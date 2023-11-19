// Shoot Them Up game. All Rights Reserved.


#include "Pickup/STUAmmoPickup.h"
#include "Weapon/STUWeaponComponent.h"
#include "Component/STUHealthComponent.h"
#include "STUUtils.h"

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
	if(!HealthComponent || HealthComponent->IsDead()) return false;
	
	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
	if(!WeaponComponent) return false;

	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}