#pragma once
#include "Menu.h"

class CreditsMenu : public Menu
{
public:
	static CreditsMenu* GetInstance();

public:
	CreditsMenu() : Menu("Credits") {}

private:
	virtual void _Tick();
};