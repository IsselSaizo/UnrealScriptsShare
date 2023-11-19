// Shoot Them Up game. All Rights Reserved.


#include "AI/Services/ChangeService.h"
#include "STUAIController.h"
#include "STUUtils.h"
#include "Weapon/STUWeaponComponent.h"

UChangeService::UChangeService()
{
	NodeName = "Change Weapon";
}

void UChangeService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	if(Controller)
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
		if(WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
		{
			WeaponComponent->SwitchWeapon();
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
