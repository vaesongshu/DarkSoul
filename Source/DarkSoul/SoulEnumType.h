// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoulEnumType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 
{
	NONE,		
	MELEE,			//È­·¨
	SWORD			//µ¶·¨
};

UENUM(BlueprintType)
enum class EMeleeState: uint8
{
	NONE,
	COMMON,			//ÆÕÍ¨×´Ì¬
	PREPARWAR		//±¸Õ½×´Ì¬
};

UENUM(BlueprintType)
enum class EPlayerBehavior : uint8
{
	NONE,
	IDLE,			//¿ÕÏÐ
	ATTACK,			//¹¥»÷
	ROLLING,		//·­¹ö
	SLIDE,			//»¬ÐÐ
	DEFENSE,		//·ÀÓù
	INJURY			//ÊÜ»÷
			
};

UENUM(BlueprintType)
enum class EEnemyBehavior : uint8
{
	NONE,
	IDLE,			//¿ÕÏÐ
	ATTACK,			//¹¥»÷
	ROLLING,		//·­¹ö
	INJURY			//ÊÜ»÷
};


UENUM(BlueprintType)
enum class EClothType : uint8
{
	NONE,			
	HAT,			//Ã±×Ó
	TOPCLOTH,		//ÉÏÒÂ
	BOTCLOTH,		//ÏÂÒÂ
	SHOES			//Ð¬×Ó
};
