#include "Menu.h"
#include "../Drawing.h"
#include "../Logging.h"
#include "MainMenu.h"

std::stack<Menu*> Menu::_OpenMenuStack;
LPD3DXFONT _CachedFontTitle = nullptr;
LPD3DXFONT _CachedFontItem = nullptr;

Menu::Menu(std::string title) : _Title(title), _SelectedItemIndex(0), _ItemsSize(0)
{
	_CheckFontCache();
}

Menu::~Menu()
{
	_FreeItems();
}

void Menu::AddOption(std::string label)
{
	Menu::AddCallbackOption(label, [](){});
}

void Menu::AddCallbackOption(std::string label, std::function<void()> callback)
{
	_Items[_ItemsSize++] = new Option(label, callback);
}

void Menu::AddSubmenuOption(std::string label, Menu* submenu)
{
	if (submenu)
	{
		_Items[_ItemsSize++] = new Option(label, [submenu]() { _OpenMenuStack.push(submenu); });
	}
}

void Menu::Tick()
{
	_FreeItems();
	_Tick();

	_CheckFontCache();

	Drawing::DrawText(_Title, .05f, .05f, _CachedFontTitle, { 255, 255, 255, 255 });

	float y = .1f;
	for (int i = 0; i < _ItemsSize; i++)
	{
		Drawing::DrawText(_Items[i]->Label, .05f, y, _CachedFontItem, i == _SelectedItemIndex ? ARGB(255, 60, 60, 200) : ARGB(255, 200, 200, 200));
		y += .05f;
	}
}

bool Menu::SendInput(int key)
{
	auto currentMenu = GetCurrentMenu();
	switch (key)
	{
	case KEY_MENU_OPEN:
		if (currentMenu)
		{
			while (!_OpenMenuStack.empty())
			{
				_OpenMenuStack.pop();
			}
		}
		else
		{
			_OpenMenuStack.push(MainMenu::GetInstance());
		}
		return true;
	case KEY_MENU_UP:
		if (currentMenu)
		{
			if (--currentMenu->_SelectedItemIndex < 0)
			{
				currentMenu->_SelectedItemIndex = currentMenu->_ItemsSize - 1;
			}
			return true;
		}
		break;
	case KEY_MENU_DOWN:
		if (currentMenu)
		{
			if (++currentMenu->_SelectedItemIndex == currentMenu->_ItemsSize)
			{
				currentMenu->_SelectedItemIndex = 0;
			}
			return true;
		}
		break;
	case KEY_MENU_BACK:
		if (currentMenu)
		{
			_OpenMenuStack.pop();
			return true;
		}
		break;
	case KEY_MENU_ENTER:
		if (currentMenu)
		{
			if (currentMenu->_ItemsSize > 0)
			{
				currentMenu->_Items[currentMenu->_SelectedItemIndex]->Callback();
			}
			return true;
		}
		break;
	}

	return false;
}

void Menu::_CheckFontCache()
{
	auto device = Memory::GetD3D9Device();
	if (!_CachedFontTitle)
	{
		D3DXCreateFont(device, 30, 20, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Arial", &_CachedFontTitle);
	}
	if (!_CachedFontItem)
	{
		D3DXCreateFont(device, 20, 10, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Arial", &_CachedFontItem);
	}
}

void Menu::ClearCachedFonts()
{
	if (_CachedFontTitle)
	{
		_CachedFontTitle->Release();
		_CachedFontTitle = nullptr;
	}
	if (_CachedFontItem)
	{
		_CachedFontItem->Release();
		_CachedFontItem = nullptr;
	}
}

void Menu::_FreeItems()
{
	for (int i = 0; i < _ItemsSize; i++)
	{
		delete _Items[i];
		_Items[i] = nullptr;
	}
	_ItemsSize = 0;
}