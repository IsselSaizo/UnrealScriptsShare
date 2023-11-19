// Shoot Them Up game. All Rights Reserved.


#include "Menu/STUMenuPlayerController.h"


void ASTUMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}