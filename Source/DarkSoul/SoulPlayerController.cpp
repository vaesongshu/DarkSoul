// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulPlayerController.h"
#include "DarkSoulHUD.h"
#include "Character/SoulBaseCharacter.h"

void ASoulPlayerController::OpenFightUI()
{
	if (ASoulBaseCharacter* MyPlayer = Cast<ASoulBaseCharacter>(GetPawn()))
	{
		MyPlayer->FindEnemyPtr();
	}
	if (ADarkSoulHUD* HUD = Cast<ADarkSoulHUD>(GetHUD()))
	{
		HUD->CreateFightMainUI();
	}
}
