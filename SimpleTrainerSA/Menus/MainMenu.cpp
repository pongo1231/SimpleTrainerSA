#include "MainMenu.h"
#include "CreditsMenu.h"
#include "PlayerMenu.h"
#include "SettingsMenu.h"
#include "../Logging.h"

MainMenu* _MainMenu = nullptr;

MainMenu* MainMenu::GetInstance()
{
	if (!_MainMenu)
	{
		_MainMenu = new MainMenu();
	}

	return _MainMenu;
}

void MainMenu::_Tick()
{
	AddSubmenuOption("Player", PlayerMenu::GetInstance());
	AddSubmenuOption("Settings", SettingsMenu::GetInstance());
	AddSubmenuOption("Credits", CreditsMenu::GetInstance());
}