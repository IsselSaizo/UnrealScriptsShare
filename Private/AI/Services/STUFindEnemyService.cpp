// Shoot Them Up game. All Rights Reserved.

#include "AI/Services/STUFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "STUAIController.h"
#include "STUUtils.h"
#include "Component/STUAIPerceptionComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
	NodeName = "Find Enemy";
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);
		if(PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosesEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}