// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUMainMenuWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* MainMenuButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnMainMenuReturn();
};
