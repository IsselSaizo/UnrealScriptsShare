// Shoot Them Up game. All Rights Reserved.


#include "AI/Decorators/STUIsAmmoNeed.h"
#include "STUAIController.h"
#include "STUUtils.h"
#include "Weapon/STUWeapon.h"
#include "Weapon/STUWeaponComponent.h"

USTUIsAmmoNeed::USTUIsAmmoNeed()
{
	NodeName = "Need Ammo";
}

bool USTUIsAmmoNeed::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if(!Controller) return false;

	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
	if(!WeaponComponent) return false;

	return WeaponComponent->NeedAmmo(WeaponType);
}