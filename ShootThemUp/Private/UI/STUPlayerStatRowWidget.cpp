// Shoot Them Up game. All Rights Reserved.


#include "UI/STUPlayerStatRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUPlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if(!PlayerNameTextBlock) return;
	PlayerNameTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetPlayerKills(const FText& Text)
{
	if(!PlayerKillsTextBlock) return;
	PlayerKillsTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetPlayerDeaths(const FText& Text)
{
	if(!PlayerDeathsTextBlock) return;
	PlayerDeathsTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetPlayerTeam(const FText& Text)
{
	if(!PlayerTeamTextBlock) return;
	PlayerTeamTextBlock->SetText(Text);
}

void USTUPlayerStatRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
	if(!PlayerIndicatorImage) return;
	PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USTUPlayerStatRowWidget::SetTeamColor(const FLinearColor Color)
{
	if(!TeamImage) return;
	TeamImage->SetColorAndOpacity(Color);
}