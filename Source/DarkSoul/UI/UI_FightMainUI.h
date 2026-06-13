// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkSoul/UI/DarkSoulWidgetRule.h"
#include "UI_FightMainUI.generated.h"

/**
 * 
 */
class UProgressBar;
class UTextBlock;
UCLASS()
class DARKSOUL_API UUI_FightMainUI : public UDarkSoulWidgetRule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_PlayerHP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_Stamina;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text_StaminaNotEnough;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* PlayerHPBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* StaminaBar;

	//显示体力不足的文字
	FTimerHandle ShowStaminaNotEnoughTip;

	void ShowStaminaText();

	void HiddenStaminaText();
};
