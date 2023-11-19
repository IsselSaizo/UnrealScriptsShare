// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

class ASTUWeapon;

UCLASS()
class SHOOTTHEMUP_API ASTUAmmoPickup : public ASTUBasePickup
{
	GENERATED_BODY()
	
public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	int32 ClipsAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	TSubclassOf<ASTUWeapon> WeaponType;


private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
