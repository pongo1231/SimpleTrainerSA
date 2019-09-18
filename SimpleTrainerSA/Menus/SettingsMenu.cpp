#include "SettingsMenu.h"
#include "../Loop.h"

SettingsMenu* _SettingsMenu = nullptr;

SettingsMenu* SettingsMenu::GetInstance()
{
	if (!_SettingsMenu)
	{
		_SettingsMenu = new SettingsMenu();
	}

	return _SettingsMenu;
}

void SettingsMenu::_Tick()
{
	Loop* loop = Loop::GetInstance();

	AddCallbackOption(!loop->Loop_ShowFPS ? "Show FPS" : "Hide FPS", [loop]() { loop->Loop_ShowFPS = !loop->Loop_ShowFPS; });
}