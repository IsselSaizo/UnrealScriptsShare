// Shoot Them Up game. All Rights Reserved.


#include "Menu/STUMenuGameModeBase.h"
#include "Menu/STUMenuHUD.h"
#include "Menu/STUMenuPlayerController.h"

ASTUMenuGameModeBase::ASTUMenuGameModeBase()
{
	PlayerControllerClass = ASTUMenuPlayerController::StaticClass();
	HUDClass = ASTUMenuHUD::StaticClass();
}