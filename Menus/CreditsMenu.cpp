#include "CreditsMenu.h"

CreditsMenu* _CreditsMenu = nullptr;

CreditsMenu* CreditsMenu::GetInstance()
{
	if (!_CreditsMenu)
	{
		_CreditsMenu = new CreditsMenu();
	}

	return _CreditsMenu;
}

void CreditsMenu::_Tick()
{
	AddOption("Made by pongo1231");
}