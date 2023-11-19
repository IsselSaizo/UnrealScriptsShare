// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USTUWeaponComponent();

	virtual void StartFire();
	void StopFire();
	virtual void SwitchWeapon();
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
	bool TryToAddAmmo(TSubclassOf<ASTUWeapon> WeaponType, int32 ClipsAmount);
	bool NeedAmmo(TSubclassOf<ASTUWeapon> WeaponType);

	void Zoom(bool Enabled);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	int32 CurrentWeaponIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipmentSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;

	UPROPERTY()
	ASTUWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray <ASTUWeapon*> Weapons;

	bool CanFire() const;
	bool CanEquip() const;
	void EquipWeapon(int32 WeaponIndex);
public:	

		
private:

	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	void SpawnWeapons();
	void AttachWeaponToSocket(ASTUWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);
	bool CanReload() const;

	void OnEmptyClip(ASTUWeapon* AmmoEmptyWeapon);
	void ChangeClip();
};
