#pragma once
#include "Menu.h"

class SettingsMenu : public Menu
{
public:
	static SettingsMenu* GetInstance();

private:
	SettingsMenu() : Menu("Settings") {}

private:
	virtual void _Tick();
};