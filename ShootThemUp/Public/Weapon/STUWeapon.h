// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUWeapon.generated.h"

class USkeletalMeshComponen;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUWeapon();

	FOnClipEmptySignature OnClipEmpty;

	virtual void StartFire();
	virtual void StopFire();
	virtual void Zoom(bool Enabled) {}

	void ChangeClips();
	bool CanReload() const;
	FWeaponUIData GetUIData() const { return UIData; }
	FAmmoData GetAmmoData() const { return CurrentAmmo; }
	bool TryToAddAmmo(int32 ClipsAmount);
	bool IsAmmoEmpty() const;
	bool IsAmmoFull() const;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TraceMaxDistance = 10000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmo{15, 10, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FWeaponUIData UIData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* MuzzleFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* FireSound;

	virtual void BeginPlay() override;
	
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	virtual void MakeShot();
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);
	void TrueHit(FHitResult& CameraHitResult, const FVector& TraceStart, const FVector& TraceEnd, const FTransform& SocketTransform);

	void DecreaseAmmo();
	bool IsClipsEmpty() const;

	UNiagaraComponent* SpawnMuzzleFX();
private:
	FAmmoData CurrentAmmo;
};
