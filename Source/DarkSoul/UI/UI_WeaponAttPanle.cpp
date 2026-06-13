// Fill out your copyright notice in the Description page of Project Settings.


#include "DarkSoul/UI/UI_WeaponAttPanle.h"
#include "../Logic/DarkSoulEventManager.h"

void UUI_WeaponAttPanle::NativeConstruct()
{
	Super::NativeConstruct();
	UDarkSoulEventManager::GetInstancePtr()->UpdateWeaponTex.BindUObject(this, &UUI_WeaponAttPanle::UpdateWeaponText);
}
