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

	AddCallbackOption(!loop->Loop_Invincibility ? "Enable Invincibility" : "Disable Invincibility",
		[loop]() { loop->Loop_Invincibility = !loop->Loop_Invincibility; });
	AddCallbackOption("Full Health", []() { *Memory::PlayerHealthAddr() = *Memory::PlayerMaxHealthAddr() / 5.f; });
	AddCallbackOption("Full Armor", []() { *Memory::PlayerArmorAddr() = 100.f; });
	AddCallbackOption("Suicide", []() { *Memory::PlayerHealthAddr() = 0.f; });
	AddCallbackOption(*Memory::PlayerMaxHealthAddr() != 1000.f ? "Max Health" : "Reset Max Health",
		[]() { *Memory::PlayerMaxHealthAddr() = *Memory::PlayerMaxHealthAddr() != 1000.f ? 1000.f : 569.f; });

}