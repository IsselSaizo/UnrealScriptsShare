// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGameDataWidget.generated.h"


class ASTUGameModeBase;
class ASTUPlayerState;

UCLASS()
class SHOOTTHEMUP_API USTUGameDataWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetKillsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetCurrentRoundNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetTotalsRoundsNum() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetRoundSecondsRemaining() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 GetDeathNum() const;

private:
	ASTUGameModeBase* GetSTUGameMode() const;
	ASTUPlayerState* GetSTUPlayerState() const;
};
