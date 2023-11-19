// Shoot Them Up game. All Rights Reserved.

#include "Sound/STUSoundFuncLib.h"
#include "Sound/SoundClass.h"

void USTUSoundFuncLib::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	if(!SoundClass) return;

	SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

void USTUSoundFuncLib::ToggleSoundClassVolume(USoundClass* SoundClass)
{
	if(!SoundClass) return;

	const auto NextVolume = SoundClass->Properties.Volume > 0.0f ? 0.0f : 1.0f;
	SetSoundClassVolume(SoundClass, NextVolume);
}