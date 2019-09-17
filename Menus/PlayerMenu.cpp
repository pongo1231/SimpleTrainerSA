#include "PlayerMenu.h"
#include "../Memory.h"

PlayerMenu* _PlayerMenu = nullptr;

PlayerMenu* PlayerMenu::GetInstance()
{
	if (!_PlayerMenu)
	{
		_PlayerMenu = new PlayerMenu();
	}

	return _PlayerMenu;
}

void PlayerMenu::_Tick()
{
	AddCallbackOption("Suicide", []() { *(float*)(Memory::GetPlayerPedBaseAddr() + 1344) = 0.f; });
}