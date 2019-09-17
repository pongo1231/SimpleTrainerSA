#include "PlayerMenu.h"
#include "../Memory.h"
#include "../Loop.h"

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
	Loop* loop = Loop::GetInstance();

	AddCallbackOption(loop->Loop_Invincibility ? "Disable Invincibility" : "Enable Invincibility",
		[loop]() { loop->Loop_Invincibility = !loop->Loop_Invincibility; });
	AddCallbackOption("Suicide", []() { Memory::SetPlayerHealth(0.f); });
}