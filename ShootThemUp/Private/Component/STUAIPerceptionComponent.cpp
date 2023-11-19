// Shoot Them Up game. All Rights Reserved.


#include "Component/STUAIPerceptionComponent.h"
#include "AI/STUAIController.h"
#include "STUUtils.h"
#include "Component/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AActor* USTUAIPerceptionComponent::GetClosesEnemy() const
{
	TArray<AActor*> PerceptionActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceptionActors);
	if(PerceptionActors.Num() == 0) 
	{
		GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerceptionActors);
		if(PerceptionActors.Num() == 0) return nullptr;
	}
	const auto Controller = Cast<AAIController>(GetOwner());
	if(!Controller) return nullptr;

	const auto Pawn = Controller->GetPawn();
	if(!Pawn) return nullptr;

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	for(const auto PerceiveActor : PerceptionActors)
	{
		const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PerceiveActor);
		
		const auto PerceivePawn = Cast<APawn>(PerceiveActor);
		const auto AreEnemies = PerceiveActor && STUUtils::AreEnemies(Controller, PerceivePawn->Controller);

		if(HealthComponent && !HealthComponent->IsDead() && AreEnemies)
		{
			const auto CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PerceiveActor;
			}
		}
	}

	return BestPawn;
}