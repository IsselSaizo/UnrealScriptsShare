// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/STUBasePickup.h"
#include "STUHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUBasePickup
{
	GENERATED_BODY()
	
public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "First Aid", meta = (ClangMin = "10", ClangMax = "100"))
	float HealthAmount = 50.0f;
private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;


};
