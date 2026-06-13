#include "DarkSoul/Logic/DarkSoulEventManager.h"


UDarkSoulEventManager* UDarkSoulEventManager::s_Instance = nullptr;
UDarkSoulEventManager* UDarkSoulEventManager::GetInstancePtr()
{
	if (s_Instance == nullptr)
	{
		s_Instance = NewObject<UDarkSoulEventManager>();
		s_Instance->AddToRoot();
	}
	return s_Instance;
}


