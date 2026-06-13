// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/UI/UI_FightMainUI.h"
#include <UMG/Public/Components/TextBlock.h>

void UUI_FightMainUI::ShowStaminaText()
{
	Text_StaminaNotEnough->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(ShowStaminaNotEnoughTip, this, &UUI_FightMainUI::HiddenStaminaText, 1.f, false, 1.f);
}

void UUI_FightMainUI::HiddenStaminaText()
{
	Text_StaminaNotEnough->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(ShowStaminaNotEnoughTip);
}
