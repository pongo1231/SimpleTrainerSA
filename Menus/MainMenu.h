#pragma once
#include "Menu.h"

class MainMenu : public Menu
{
public:
	static MainMenu* GetInstance();

private:
	MainMenu() : Menu("Main Menu") {}

private:
	virtual void _Tick();
};