// Copyright Epic Games, Inc. All Rights Reserved.


#include "DarkSoulGameModeBase.h"
#include "SoulPlayerController.h"
#include "Character/SoulBaseCharacter.h"
#include "DarkSoulHUD.h"

ADarkSoulGameModeBase::ADarkSoulGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ASoulBaseCharacter> BPPlayerClass(TEXT("/Game/_Game/BP/BP_Player"));
	if (BPPlayerClass.Class)
	{
		DefaultPawnClass = BPPlayerClass.Class;
	}
	
	PlayerControllerClass = ASoulPlayerController::StaticClass();

	HUDClass = ADarkSoulHUD::StaticClass();
}
