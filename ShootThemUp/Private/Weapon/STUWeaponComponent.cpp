// Shoot Them Up game. All Rights Reserved.


#include "Weapon/STUWeaponComponent.h"
#include "Weapon/STUWeapon.h"
#include "GameFramework/Character.h"
#include "Notifies/STUEquipFinishedAnimNotify.h"
#include "Notifies/STUReloadAnimNotify.h"
#include "Notifies/AnimUtils.h"

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}


void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);

	CurrentWeaponIndex = 0;
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
	InitAnimations();
}

void USTUWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character || !GetWorld()) return;

	for(auto OneWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUWeapon>(OneWeaponData.WeaponClass);
		if(!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void USTUWeaponComponent::StartFire()
{
	if(!CanFire()) return;
	CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
	if(!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if(!Weapon || !SceneComponent) return;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid weapon index"));
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character) return;

	if(CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		CurrentWeapon->Zoom(false);
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data){//
		return Data.WeaponClass == CurrentWeapon->GetClass();
		});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipmentSocketName);
	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::SwitchWeapon()
{
	if(!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for(auto Weapon:Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character) return;
	Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if(EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for(auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedAnimNotify = AnimUtils::FindNotifyByClass<USTUReloadAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if(!ReloadFinishedAnimNotify)
		{
			UE_LOG(LogTemp, Error, TEXT("Reload anim notify is forgotten to set"));
			checkNoEntry();
		}
		ReloadFinishedAnimNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character || MeshComp != Character->GetMesh()) return;

	EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if(!Character || MeshComp != Character->GetMesh()) return;

	ReloadAnimInProgress = false;
}

void USTUWeaponComponent::Reload()
{
	ChangeClip();
}

void USTUWeaponComponent::OnEmptyClip(ASTUWeapon* AmmoEmptyWeapon)
{
	if(!AmmoEmptyWeapon) return;

	if(CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for(const auto Weapon: Weapons)
		{
			if(Weapon == AmmoEmptyWeapon)
			{
				ChangeClip();
			}
		}
	}
}

void USTUWeaponComponent::ChangeClip()
{
	if(!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClips();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

bool USTUWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
	return CurrentWeapon//
		   && !EquipAnimInProgress//
		   && !ReloadAnimInProgress//
		   && CurrentWeapon->CanReload();
}

bool USTUWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if(CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if(CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUWeapon> WeaponType, int32 ClipsAmount)
{
	for(const auto Weapon: Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}

bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUWeapon> WeaponType)
{
	for(const auto Weapon: Weapons)
	{
	 if(Weapon && Weapon->IsA(WeaponType))
	 {
		return !Weapon->IsAmmoFull();
	 }
	}
	return false;
}

void USTUWeaponComponent::Zoom(bool Enabled)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->Zoom(Enabled);
	}
}