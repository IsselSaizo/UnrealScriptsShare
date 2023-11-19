// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUWeapon.h"
#include "STULauncher.generated.h"

class ASTUProjectile;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTULauncher : public ASTUWeapon
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ASTUProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoAmmoSound;

protected:

	virtual void MakeShot() override;
};
