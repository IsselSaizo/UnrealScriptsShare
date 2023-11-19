// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ChangeService.generated.h"


UCLASS()
class SHOOTTHEMUP_API UChangeService : public UBTService
{
	GENERATED_BODY()
	
public:
	UChangeService();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Probability = 0.8f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
