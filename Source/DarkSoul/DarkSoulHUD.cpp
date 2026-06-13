// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoulHUD.h"
#include "UI/UI_FightMainUI.h"
#include "UI/UI_FightResult.h"
#include "Logic/DarkSoulEventManager.h"
#include "UI/UI_MainMapUI.h"
#include <Kismet/GameplayStatics.h>
#include "Character/SoulBaseCharacter.h"
#include "DrakSoulGameInstance.h"

ADarkSoulHUD::ADarkSoulHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_FightMainUI> UI_FightMainUIClass(TEXT("/Game/_Game/BP/UI/BPUI_FightMain"));
	static ConstructorHelpers::FClassFinder<UUI_FightResult> UI_FightUIResultClass(TEXT("/Game/_Game/BP/UI/BPUI_FightResult"));
	static ConstructorHelpers::FClassFinder<UUI_MainMapUI> UI_MainMapClass(TEXT("/Game/_Game/BP/UI/BPUI_MainMap"));
	FightMainUIClass = UI_FightMainUIClass.Class;
	FightResultUIClass = UI_FightUIResultClass.Class;
	MainMapUIClass = UI_MainMapClass.Class;
}

void ADarkSoulHUD::BeginPlay()
{
	Super::BeginPlay();
	UDarkSoulEventManager::GetInstancePtr()->OpenFightResultUI.BindUObject(this, &ADarkSoulHUD::CreateFightResult);
	if (UDrakSoulGameInstance* GI = Cast<UDrakSoulGameInstance>(GetGameInstance()))
	{		
		if (GI->IsInFightMap == false)
		{
			CreateMainMapUI();
		}
	}
}

UUI_FightMainUI* ADarkSoulHUD::GetFightMainUI()
{
	return FightMainUI;
}

void ADarkSoulHUD::CreateFightMainUI()
{
	FightMainUI = CreateWidget<UUI_FightMainUI>(GetWorld(), FightMainUIClass);
 	FightMainUI->AddToViewport(0);
	if (ASoulBaseCharacter* MyPlayer = Cast<ASoulBaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		UDarkSoulEventManager::GetInstancePtr()->UpdateWeaponTex.ExecuteIfBound(MyPlayer->WeaponType);
	}
}

void ADarkSoulHUD::CreateFightResult(bool IsWin)
{
	FightMainUI->RemoveFromParent();
	FightResultUI = CreateWidget<UUI_FightResult>(GetWorld(), FightResultUIClass);
	FightResultUI->SetResultText(IsWin);
	FightResultUI->AddToViewport(0);
}

void ADarkSoulHUD::CreateMainMapUI()
{
	MainMapUI = CreateWidget<UUI_MainMapUI>(GetWorld(), MainMapUIClass);
	MainMapUI->AddToViewport(0);
	if (ASoulBaseCharacter* MyPlayer = Cast<ASoulBaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		UDarkSoulEventManager::GetInstancePtr()->UpdateWeaponTex.ExecuteIfBound(MyPlayer->WeaponType);
	}
}
