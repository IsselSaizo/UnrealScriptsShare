// Shoot Them Up game. All Rights Reserved.


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFXComponent.h"


ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.1f;
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}


void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();

	check(MovementComponent);
	check(CollisionComponent);
	check(WeaponFXComponent);
	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit);
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	SetLifeSpan(LifeTime);
}

void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(!GetWorld()) return;
	UE_LOG(LogTemp, Display, TEXT("It`s true"));
	MovementComponent->StopMovementImmediately();
	///make damage радиал
	UGameplayStatics::ApplyRadialDamage(GetWorld(),	//Получаем ворлд
		DamageAmount,								//Выставляем урон в центре
		GetActorLocation(),							//получаем координаты актора
		DamageRadius,								//Выставляем радиус
		UDamageType::StaticClass(),					//Присваеваем базовый тип наносимому урону
		{GetOwner()},								//Акторы, которым не наносится ущерб
		this,										//
		GetController(),									//
		DoFullDamage);								//

	//DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
	WeaponFXComponent->PlayImpactFX(Hit);
	Destroy();
}

AController* ASTUProjectile::GetController() const //Понадобится для посчёта кто кого подстрелил
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}