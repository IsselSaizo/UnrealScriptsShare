// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

class USTUHealthComponent;
class USTUWeaponComponent;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASTUBaseCharacter(const FObjectInitializer& ObjInit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USTUHealthComponent* HealthComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpawnOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    USTUWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorName = "Paint Color";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSound;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void OnHealthChanged(float Health, float HealthDelta);

public:
    virtual void Tick(float DeltaTime) override;
    virtual void OnDeath();
    virtual void TurnOff() override;
    virtual void Reset() override;
    void SetPlayerColor(const FLinearColor& Color);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;


private:
    void SpawnWeapon();

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);
};
