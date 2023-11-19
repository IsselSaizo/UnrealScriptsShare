// Shoot Them Up game. All Rights Reserved.


#include "Weapon/STURiffle.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


ASTURiffle::ASTURiffle()
{
	WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURiffle::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponFXComponent);
}

void ASTURiffle::StartFire()
{
	InitFX();
	GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTURiffle::MakeShot, TimeBetweenShoots, true);
	MakeShot();
}

void ASTURiffle::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
	SetFXActive(false);
}


void ASTURiffle::MakeShot()
{
	if(!GetWorld() || IsAmmoEmpty()) 
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if(!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	TrueHit(HitResult, TraceStart, TraceEnd, SocketTransform);

	FVector TraceFXEnd = TraceEnd;
	if(HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
	}
	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
	DecreaseAmmo();
}

bool ASTURiffle::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const 
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation; //SocketTransform.GetLocation(); Под комментом для стрельбы от оружия, вне - от камеры
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad); //SocketTransform.GetRotation().GetForwardVector(); аналогично, только с поворотом
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTURiffle::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if(!DamagedActor) return;
	
	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = HitResult;
	DamagedActor->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);
}

void ASTURiffle::InitFX()
{
	if(!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}

	if(!FireAudioComponent)
	{
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
	}

	SetFXActive(true);
}

void ASTURiffle::SetFXActive(bool IsActive)
{
	if(MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!IsActive);
		MuzzleFXComponent->SetVisibility(IsActive, true);
	}

	if(FireAudioComponent)
	{
		FireAudioComponent->SetPaused(!IsActive);
	}
}

void ASTURiffle::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if(TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

AController* ASTURiffle::GetController() const //Понадобится для посчёта кто кого подстрелил
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}

void ASTURiffle::Zoom(bool Enabled)
{
	const auto Controller = Cast<APlayerController>(GetController());
	if(!Controller || !Controller->PlayerCameraManager) return;

	if(Enabled)
	{
		DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
	}

	Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
}