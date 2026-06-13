// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkSoul/UI/DarkSoulWidgetRule.h"
#include "../SoulEnumType.h"
#include "UI_WeaponAttPanle.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UUI_WeaponAttPanle : public UDarkSoulWidgetRule
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWeaponText(EWeaponType WT);
	
};
