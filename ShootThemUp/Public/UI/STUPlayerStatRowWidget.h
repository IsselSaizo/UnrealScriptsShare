// Shoot Them Up game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerStatRowWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class SHOOTTHEMUP_API USTUPlayerStatRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetPlayerName(const FText& Text);
	void SetPlayerKills(const FText& Text);
	void SetPlayerDeaths(const FText& Text);
	void SetPlayerTeam(const FText& Text);
	void SetPlayerIndicatorVisibility(bool Visible);
	void SetTeamColor(const FLinearColor Color);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerKillsTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerDeathsTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerTeamTextBlock;

	UPROPERTY(meta = (BindWidget))
	UImage* PlayerIndicatorImage;

	UPROPERTY(meta = (BindWidget))
	UImage* TeamImage;
};
