// Shoot Them Up game. All Rights Reserved.


#include "Menu/STUMenuHUD.h"
#include "UI/STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if(MenuWidgetClass)
	{
		const auto MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass);
		if(MenuWidget)
		{
			MenuWidget->AddToViewport();
			MenuWidget->Show();
		}
	}
}