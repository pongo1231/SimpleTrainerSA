#pragma once
#include <string>
#include <functional>
#include <vector>
#include <Windows.h>
#include <stack>

#define KEY_MENU_OPEN VK_OEM_MINUS
#define KEY_MENU_UP VK_UP
#define KEY_MENU_DOWN VK_DOWN
#define KEY_MENU_ENTER VK_RETURN
#define KEY_MENU_BACK VK_BACK

#define MAX_ITEMS 64

class Menu
{
public:
	static bool SendInput(int key);
	static void ClearCachedFonts();

	static inline Menu* GetCurrentMenu()
	{
		return _OpenMenuStack.empty() ? nullptr : _OpenMenuStack.top();
	}

private:
	static std::stack<Menu*> _OpenMenuStack;

protected:
	Menu(std::string title);
	~Menu();

public:
	void AddOption(std::string label);
	void AddCallbackOption(std::string label, std::function<void()> callback);
	void AddSubmenuOption(std::string label, Menu* subMenu);
	void Tick();

private:
	struct Option
	{
		Option(std::string label, std::function<void()> callback) : Label(label), Callback(callback) {}

		const std::string Label;
		const std::function<void()> Callback;
	};

	std::string _Title;
	Option* _Items[MAX_ITEMS] = { nullptr };
	int _ItemsSize;
	int _SelectedItemIndex;

	void _CheckFontCache();
	void _FreeItems();

	virtual void _Tick() = 0;
};

