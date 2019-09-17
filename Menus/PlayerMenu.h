#pragma once
#include "Menu.h"

class PlayerMenu : public Menu
{
public:
	static PlayerMenu* GetInstance();

private:
	PlayerMenu() : Menu("Player Menu") {}

private:
	virtual void _Tick();
};