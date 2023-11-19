// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTURespawnComponent();

	void Respawn(int32 RespawnTime);
	int32 GetRespawnCountDown() const { return RespawnCountDown; }
	bool IsRespawnInProgres() const;

private:
	FTimerHandle RespawnTimerHandle;
	int32 RespawnCountDown = 0;

	void RespawnTimerUpdate();
};
