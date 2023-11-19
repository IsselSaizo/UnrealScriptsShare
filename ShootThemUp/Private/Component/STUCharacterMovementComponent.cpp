// Shoot Them Up game. All Rights Reserved.


#include "Component/STUCharacterMovementComponent.h"
#include "Player/STUBaseCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ASTUBaseCharacter* Player = Cast<ASTUBaseCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}