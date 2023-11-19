// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUIsAmmoNeed.generated.h"

class ASTUWeapon;

UCLASS()
class SHOOTTHEMUP_API USTUIsAmmoNeed : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<ASTUWeapon> WeaponType;

	USTUIsAmmoNeed();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
