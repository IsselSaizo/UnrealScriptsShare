// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerStatBox;

	UPROPERTY(meta = (BindWidget))
	UButton* ResetLevelButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

	virtual void NativeOnInitialized() override;
private:
	void OnMatchStateChanged(ESTUMatchState State);
	void UpdatePlayerStat();

	UFUNCTION()
	void OnResetLevel();
};
