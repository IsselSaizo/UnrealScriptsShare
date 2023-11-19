// Shoot Them Up game. All Rights Reserved.

#include "AI/EQS/EnvQueryTest_PickupCanBeTaken.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"
#include "Pickup/STUBasePickup.h"

UEnvQueryTest_PickupCanBeTaken::UEnvQueryTest_PickupCanBeTaken(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_PickupCanBeTaken::RunTest(FEnvQueryInstance& QueryInstance) const
{
	const auto DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);
	const bool WantsBeTakable = BoolValue.GetValue();

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const auto ItemActor = GetItemActor(QueryInstance, It.GetIndex());
		const auto PickupActor = Cast<ASTUBasePickup>(ItemActor);
		if(!PickupActor) continue;

		const auto CouldBeTaken = PickupActor->CouldBeTaken();
		It.SetScore(TestPurpose, FilterType, CouldBeTaken, WantsBeTakable);
	}
}