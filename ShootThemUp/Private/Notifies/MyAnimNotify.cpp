// Shoot Them Up game. All Rights Reserved.


#include "Notifies/MyAnimNotify.h"

void UMyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}