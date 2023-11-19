// Shoot Them Up game. All Rights Reserved.


#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "Component/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(GetOwningPlayer());
	if(!RespawnComponent || !RespawnComponent->IsRespawnInProgres()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}