// Shoot Them Up game. All Rights Reserved.


#include "Weapon/STUWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASTUWeapon::ASTUWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}


void ASTUWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullet`s count couldn`t be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Bullet`s count couldn`t be less or equal zero"));
	CurrentAmmo = DefaultAmmo;
}

void ASTUWeapon::StartFire()
{
	
}

void ASTUWeapon::StopFire()
{

}


void ASTUWeapon::MakeShot()
{
	
}

bool ASTUWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const 
{
	const auto STUCharacter = Cast<ACharacter>(GetOwner());
	if(!STUCharacter) return false;

	if(STUCharacter->IsPlayerControlled())
	{
		const auto Controller = STUCharacter->GetController<APlayerController>();
		if(!Controller) return false;

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}
	return true;
}

FVector ASTUWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const 
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation; //SocketTransform.GetLocation(); Под комментом для стрельбы от оружия, вне - от камеры
	const FVector ShootDirection = ViewRotation.Vector(); //SocketTransform.GetRotation().GetForwardVector(); аналогично, только с поворотом
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ASTUWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if(!GetWorld()) return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

}

void ASTUWeapon::TrueHit(FHitResult& CameraHitResult, const FVector& TraceStart, const FVector& TraceEnd, const FTransform& SocketTransform)
{
	const FVector ActualTraceEnd = CameraHitResult.bBlockingHit ? CameraHitResult.ImpactPoint : CameraHitResult.TraceEnd;
	const FVector HitDirectionFromMuzzle = (ActualTraceEnd - SocketTransform.GetLocation()).GetSafeNormal();
	const float Degrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SocketTransform.GetRotation().GetForwardVector(), HitDirectionFromMuzzle)));
	//UE_LOG(LogTemp, Display, TEXT("degree is %.0f"), Degrees); //дебаг инфа о градусах выстрела от дула к камере
	if(Degrees > 90.0f) 
	{
		CameraHitResult.bBlockingHit = false;
		return;
	}
}

void ASTUWeapon::DecreaseAmmo()
{
	if(CurrentAmmo.Bullets == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Clip is empty"));
			return;
		}
	CurrentAmmo.Bullets--;
	if(IsClipsEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast(this);
	}
}

bool ASTUWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipsEmpty();
}

bool ASTUWeapon::IsClipsEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

void ASTUWeapon::ChangeClips()
{
	if(!CurrentAmmo.Infinite)
	{
		if(CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No more clips"));
			return;
		}
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool ASTUWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

bool ASTUWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && //
		CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

bool ASTUWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	if(CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

	if(IsAmmoEmpty())
	{
		CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
		OnClipEmpty.Broadcast(this);
	}
	else if(CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
		if(DefaultAmmo.Clips - NextClipsAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipsAmount;
		}
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}
	return true;
}

UNiagaraComponent* ASTUWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,//
		WeaponMesh,//
		MuzzleSocketName,//
		FVector::ZeroVector,//
		FRotator::ZeroRotator,//
		EAttachLocation::SnapToTarget, true);
}