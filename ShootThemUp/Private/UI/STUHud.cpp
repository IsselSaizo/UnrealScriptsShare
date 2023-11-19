// Shoot Them Up game. All Rights Reserved.


#include "UI/STUHud.h"
#include "Engine/Canvas.h"
#include "UI/STUBaseWidget.h"
#include "STUGameModeBase.h"

void ASTUHud::DrawHUD()
{
	Super::DrawHUD();

	//DrawCrossHair();
}

void ASTUHud::BeginPlay()
{
	Super::BeginPlay();

	GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass));

	for(auto GameWidgetPair: GameWidgets)
	{
		const auto GameWidget = GameWidgetPair.Value;
		if(!GameWidget) continue;

		GameWidget->AddToViewport();
		GameWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if(GetWorld())
	{
		const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
		if(GameMode)
		{
			GameMode->OnMatchStateChange.AddUObject(this, &ASTUHud::OnMatchStateChanged);
		}
	}
}

void ASTUHud::DrawCrossHair()
{
	int32 SizeX = Canvas->SizeX;
	int32 SizeY = Canvas->SizeY;
	const TInterval<float> Center(SizeX * 0.5f, SizeY * 0.5f);

	const float HalfLineSize = 10.0f;
	const float LineThickness = 2.0f;
	const FLinearColor LineColor = FLinearColor::Green;

	DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
	DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

void ASTUHud::OnMatchStateChanged(ESTUMatchState State)
{
	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if(GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets [State];
	}

	if(CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
	}

	UE_LOG(LogTemp, Display, TEXT("Match State Change %s"), *UEnum::GetValueAsString(State))
}