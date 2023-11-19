// Shoot Them Up game. All Rights Reserved.


#include "Component/STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"
#include "STUGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);

	AActor* ComponentOwner = GetOwner();
	if(ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage
(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	
}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, 
	class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, 
	FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser )
{
	const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
	ApplyDamage(FinalDamage, InstigatedBy);
}

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, 
	const class UDamageType* DamageType, FVector Origin, FHitResult HitInfo, class AController* InstigatedBy, 
	AActor* DamageCauser)
{
	ApplyDamage(Damage, InstigatedBy);
}

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if(Damage <= 0.0f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if(IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)
{
	const auto Character = Cast<ACharacter>(DamagedActor);
	if(!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName)) return 1.0f;

	const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if(!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.0f;

	return DamageModifiers[PhysMaterial];
}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if(IsHealthFull() && GetWorld()) 
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}

}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if(IsHealthFull() || IsDead()) return false;

	SetHealth(Health + HealthAmount);
	return true;
}

bool USTUHealthComponent::IsHealthFull()
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::PlayCameraShake()
{
	if(IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if(!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::Killed(AController* KillerController)
{
	if(!GetWorld()) return;
	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
	if(!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->Controller : nullptr;

	GameMode->Killed(KillerController, VictimController);
}

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	if(!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
}