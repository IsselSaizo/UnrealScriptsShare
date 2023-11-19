// Shoot Them Up game. All Rights Reserved.


#include "UI/STUMainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"


void USTUMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &USTUMainMenuWidget::OnMainMenuReturn);
	}
}

void USTUMainMenuWidget::OnMainMenuReturn()
{
	if(!GetWorld()) return;

	const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
	if(!STUGameInstance) return;

	if(STUGameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("Menu Level Name is None"));
		return;
	}
	UGameplayStatics::OpenLevel(this, STUGameInstance->GetMenuLevelName());
}