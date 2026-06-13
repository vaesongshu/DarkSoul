// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DarkSoulHUD.generated.h"

/**
 * 
 */
class UUI_FightMainUI;
class UUI_FightResult;
class UUI_MainMapUI;
UCLASS()
class DARKSOUL_API ADarkSoulHUD : public AHUD
{
	GENERATED_BODY()

	ADarkSoulHUD();

	virtual void BeginPlay() override;

	TSubclassOf<UUI_FightMainUI> FightMainUIClass;
	TSubclassOf<UUI_FightResult> FightResultUIClass;
	TSubclassOf<UUI_MainMapUI> MainMapUIClass;

	UUI_FightMainUI* FightMainUI;
	UUI_FightResult* FightResultUI;
	UUI_MainMapUI* MainMapUI;

public:
	UUI_FightMainUI* GetFightMainUI();

	void CreateFightMainUI();

	UFUNCTION()
	void CreateFightResult(bool IsWin);

	void CreateMainMapUI();
	
};
