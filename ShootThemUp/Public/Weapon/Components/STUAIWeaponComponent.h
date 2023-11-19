// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUWeaponComponent.h"
#include "STUAIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAIWeaponComponent : public USTUWeaponComponent
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;
	virtual void SwitchWeapon() override;
};
