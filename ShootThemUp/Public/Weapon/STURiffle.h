// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUWeapon.h"
#include "STURiffle.generated.h"

class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class SHOOTTHEMUP_API ASTURiffle : public ASTUWeapon
{
	GENERATED_BODY()
	
public:
	ASTURiffle();

	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void Zoom(bool Enabled) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Accuracity")
	float TimeBetweenShoots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Accuracity")
	float BulletSpread = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Accuracity")
	float FOVZoomAngle = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	USTUWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
	void MakeDamage(const FHitResult& HitResult);

private:
	FTimerHandle ShootTimerHandle;

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;

	UPROPERTY()
	UAudioComponent* FireAudioComponent;

	void InitFX();
	void SetFXActive(bool IsActive);
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
	AController* GetController() const;
	
	float DefaultCameraFOV = 90.0f;
};