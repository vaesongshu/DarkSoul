// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkSoul/UI/DarkSoulWidgetRule.h"
#include "UI_FightResult.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API UUI_FightResult : public UDarkSoulWidgetRule
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	int32 CountDownTime;
	float WaitTime;

	UFUNCTION(BlueprintCallable,BlueprintPure)
	int32 GetCountDownTime();

	UFUNCTION(BlueprintImplementableEvent)
	void SetResultText(bool IsWin);

	UFUNCTION(BlueprintImplementableEvent)
	void SetCountDownText();
};
