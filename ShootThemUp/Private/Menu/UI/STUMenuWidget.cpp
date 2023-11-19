// Shoot Them Up game. All Rights Reserved.


#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Menu/UI/STULevelItemWidget.h"
#include "Sound/SoundCue.h"

void USTUMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
	}

	if(QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
	}

	InitLevelItems();
}

void USTUMenuWidget::InitLevelItems()
{
	const auto STUGameInstance = GetSTUGameInstance();
	if(!STUGameInstance) return;

	checkf(STUGameInstance->GetLevelsData().Num() != 0, TEXT("Array of levels shouldn`t be empty"));

	if(!LevelItemsBox) return;
	LevelItemsBox->ClearChildren();

	for(auto LevelData: STUGameInstance->GetLevelsData())
	{
		const auto LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if(!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &USTUMenuWidget::OnLevelSelected);

		LevelItemsBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}

	if(STUGameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(STUGameInstance->GetLevelsData()[0]);
	}else
	{
		OnLevelSelected(STUGameInstance->GetStartupLevel());
	}
}

void USTUMenuWidget::OnLevelSelected(const FLevelData& Data)
{
	const auto STUGameInstance = GetSTUGameInstance();
	if(!STUGameInstance) return;

	STUGameInstance->SetStartupLevel(Data);

	for(auto LevelItemWidget: LevelItemWidgets)
	{
		if(LevelItemWidget)
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(IsSelected);
		}
	}
}

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if(Animation != HideAnimation) return;

	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if(!STUGameInstance) return;

	UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevel().LevelName);
}

void USTUMenuWidget::OnStartGame()
{
	PlayAnimation(HideAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void  USTUMenuWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

USTUGameInstance* USTUMenuWidget::GetSTUGameInstance() const
{
	if(!GetWorld()) return nullptr;
	return GetWorld()->GetGameInstance<USTUGameInstance>();
}