// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUCoreTypes.h"
#include "STUHud.generated.h"

class USTUBaseWidget;

UCLASS()
class SHOOTTHEMUP_API ASTUHud : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<ESTUMatchState, USTUBaseWidget*> GameWidgets;

	UPROPERTY()
	USTUBaseWidget* CurrentWidget = nullptr;

	void DrawCrossHair();
	void OnMatchStateChanged(ESTUMatchState State);
};
