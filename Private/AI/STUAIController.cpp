// Shoot Them Up game. All Rights Reserved.

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "Component/STUAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/STURespawnComponent.h"

ASTUAIController::ASTUAIController()
{
	STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);

	RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
	bWantsPlayerState = true;
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto STUCharacter = Cast<ASTUAICharacter>(InPawn);
	if(STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTreeAsset);
	}
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);

}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if(!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}