// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/UI/UI_FightResult.h"
#include <Kismet/GameplayStatics.h>
#include "../DrakSoulGameInstance.h"

void UUI_FightResult::NativeConstruct()
{
	Super::NativeConstruct();

	CountDownTime = 5;
	WaitTime = 0.f;
	SetCountDownText();
}

void UUI_FightResult::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CountDownTime > 0)
	{
		WaitTime = WaitTime + InDeltaTime;
		if (WaitTime >= 1)
		{
			CountDownTime--;
			SetCountDownText();
			WaitTime = 0;
		}
		if (CountDownTime == 0)
		{
			if (UDrakSoulGameInstance* GI = Cast<UDrakSoulGameInstance>(GetGameInstance()))
			{
				GI->IsInFightMap = false;
			}
			UGameplayStatics::OpenLevel(GetWorld(), "MainMap");
		}
	}

}

int32 UUI_FightResult::GetCountDownTime()
{
	return CountDownTime;
}
