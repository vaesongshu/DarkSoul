#pragma once

#include "DarkSoulEventManager.generated.h"

DECLARE_DELEGATE_OneParam(FOpenFightResultUI, bool);
DECLARE_DELEGATE_OneParam(FUpdateWeaponText, EWeaponType);
UCLASS(BlueprintType)
class UDarkSoulEventManager : public UObject 
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category = DarkSoulEventManager)
	static UDarkSoulEventManager* GetInstancePtr();


private:
	static UDarkSoulEventManager* s_Instance;

public:
	FOpenFightResultUI OpenFightResultUI;

	FUpdateWeaponText UpdateWeaponTex;

};