// Shoot Them Up game. All Rights Reserved.


#include "Player/STUPlayerState.h"
#include "STUUtils.h"


void ASTUPlayerState::LogInfo()
{
	UE_LOG(LogTemp, Display, TEXT("Team ID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}