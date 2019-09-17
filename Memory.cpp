#include "Memory.h"
#include "Drawing.h"
#include "Menus/Menu.h"
#include "Logging.h"
#include "minhook/MinHook.h"
#include <iostream>
#include <fstream>
#include <d3d9.h>
#include <d3dx9.h>

DWORD Memory::Base;
DWORD Memory::End;

IDirect3D9* Memory::_D3d9;
IDirect3DDevice9* Memory::_D3d9Device;
HWND* Memory::_Hwnd;

int(__stdcall* _ShowCursor)(BOOL bShow);

HRESULT(__stdcall* Orig_BeginScene)(IDirect3DDevice9* device);
HRESULT(__stdcall* Orig_EndScene)(IDirect3DDevice9* device);
HRESULT(__stdcall* Orig_Reset)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
int(__stdcall* Orig_WindowProc)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

DWORD* GameLoadState;
bool Ready = false;
bool* IsInPause;

HRESULT __stdcall Hook_BeginScene(IDirect3DDevice9* device)
{
	return Orig_BeginScene(device);
}

HRESULT __stdcall Hook_EndScene(IDirect3DDevice9* device)
{
	if (*GameLoadState == 9 && !*IsInPause)
	{
		if (!Ready)
		{
			Logging::Log << "Game loaded in, time to start drawing" << std::endl;
			Ready = true;
		}

		Drawing::NewFrame();
	}

	return Orig_EndScene(device);
}

HRESULT __stdcall Hook_Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return Orig_Reset(device, pPresentationParameters);
}

int __stdcall Hook_WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Ready)
	{
		static bool OnHold = false;

		switch (msg)
		{
		case WM_KEYDOWN:
			if (!OnHold)
			{
				switch (wParam)
				{
				case KEY_MENU_OPEN:
					if (Menu::SendInput(KEY_MENU_OPEN))
					{
						OnHold = true;
						return 0;
					}
					break;
				case KEY_MENU_UP:
					if (Menu::SendInput(KEY_MENU_UP))
					{
						OnHold = true;
						return 0;
					}
					break;
				case KEY_MENU_DOWN:
					if (Menu::SendInput(KEY_MENU_DOWN))
					{
						OnHold = true;
						return 0;
					}
					break;
				case KEY_MENU_ENTER:
					if (Menu::SendInput(KEY_MENU_ENTER))
					{
						OnHold = true;
						return 0;
					}
					break;
				case KEY_MENU_BACK:
					if (Menu::SendInput(KEY_MENU_BACK))
					{
						OnHold = true;
						return 0;
					}
					break;
				}
			}
			break;
		case WM_KEYUP:
			if (OnHold)
			{
				switch (wParam)
				{
				case KEY_MENU_OPEN:
				case KEY_MENU_UP:
				case KEY_MENU_DOWN:
				case KEY_MENU_ENTER:
				case KEY_MENU_BACK:
					OnHold = false;
					return 0;
				}
			}
			break;
		}
	}

	return Orig_WindowProc(hWnd, msg, wParam, lParam);
}

void Memory::Init()
{
	Logging::Log << "Initializing or something" << std::endl;

	MH_Initialize();

	DWORD addr;

	addr = Memory::FindPattern("\xA1\x00\x00\x00\x00\x68\x00\x00\x00\x00\x6A\x03\x8B\x08\x56\x52\x8B\x15\x00\x00\x00\x00\x52\x8B\x15\x00\x00\x00\x00\x52\x50\xFF\x51\x28\x85\xC0\x7C\x07", "x????x????xxxxxxxx????xxx????xxxxxxxxx");
	while (!**(DWORD**)(addr + 1))
	{
		Sleep(100);
	}
	_D3d9 = *(IDirect3D9**)*(DWORD*)(addr + 1);
	Logging::Log << "Found D3D9Interface: " << _D3d9 << std::endl;

	addr = Memory::FindPattern("\x8B\x4C\x24\x10\xA1\x00\x00\x00\x00\x8B\x10", "xxxxx????xx");
	while (!**(DWORD**)(addr + 5))
	{
		Sleep(100);
	}
	_D3d9Device = *(IDirect3DDevice9**)*(DWORD*)(addr + 5);
	Logging::Log << "Found D3D9Device: " << _D3d9Device << std::endl;

	addr = Memory::FindPattern("\x8B\x15\x00\x00\x00\x00\x8D\x4C\x24\x18", "xx????xxxx");
	while (!**(DWORD**)(addr + 2))
	{
		Sleep(100);
	}
	_Hwnd = *(HWND**)*(DWORD*)(addr + 2);
	Logging::Log << "Found window: " << _Hwnd << std::endl;

	addr = Memory::FindPattern("\xE8\x00\x00\x00\x00\x8B\xF8\x89\x7D\x94", "x????xxxxx");
	addr += 5 + *(DWORD*)(addr + 1);
	addr += 0x5F5;
	GameLoadState = *(DWORD**)(addr + 2);
	Logging::Log << "Found Game Load State flag: " << GameLoadState << std::endl;
	Logging::Log << "Skipping intro sequence" << std::endl;
	_SkipIntroSequence();

	addr = Memory::FindPattern("\x75\x28\x88\x0D\x00\x00\x00\x00", "xxxx????");
	IsInPause = *(bool**)(addr + 4);
	Logging::Log << "Found Pause flag: " << IsInPause << std::endl;

	addr = Memory::FindPattern("\xFF\x15\x00\x00\x00\x00\xBB\x00\x00\x00\x00", "xx????x????");
	_ShowCursor = *(int(__stdcall**)(BOOL))(addr + 3);
	Logging::Log << "Found ShowCursor function: " << _ShowCursor << std::endl;

	Logging::Log << "Hooking D3D9Device functions..." << std::endl;
	auto status = MH_CreateHook(*(DWORD**)(*(DWORD*)_D3d9Device + 164), &Hook_BeginScene, (void**)&Orig_BeginScene);
	if (status != MH_OK)
	{
		Logging::Log << "Hooking failed :/" << std::endl;
	}
	status = MH_CreateHook(*(DWORD**)(*(DWORD*)_D3d9Device + 168), &Hook_EndScene, (void**)&Orig_EndScene);
	if (status != MH_OK)
	{
		Logging::Log << "Hooking failed :/" << std::endl;
	}
	status = MH_CreateHook(*(DWORD**)(*(DWORD*)_D3d9Device + 64), &Hook_Reset, (void**)&Orig_Reset);
	if (status != MH_OK)
	{
		Logging::Log << "Hooking failed :/" << std::endl;
	}

	Logging::Log << "Hooking Window Input function..." << std::endl;
	auto windowProc = Memory::FindPattern("\x83\xEC\x3C\x53\x55\x8B\x6C\x24\x54", "xxxxxxxxx");
	status = MH_CreateHook((DWORD*)windowProc, &Hook_WindowProc, (void**)&Orig_WindowProc);
	if (status != MH_OK)
	{
		Logging::Log << "Hooking failed :/" << std::endl;
	}

	MH_EnableHook(MH_ALL_HOOKS);
	Logging::Log << "Init done!" << std::endl;
}

DWORD Memory::FindPattern(const char* pattern, const char* mask)
{
	int len = strlen(mask);
	int i = 0;
	for (DWORD addr = Base; addr < End; addr++)
	{
		if (mask[i] == '?' || *(char*)addr == pattern[i])
		{
			i++;
		}
		else
		{
			i = 0;
		}

		if (i == len)
		{
			return addr - i + 1;
		}
	}

	return 0;
}

void Memory::_SkipIntroSequence()
{
	if (*GameLoadState < 3)
	{
		*GameLoadState = 3;
	}
	while (*GameLoadState != 4)
	{
		Sleep(100);
	}
	if (*GameLoadState == 4)
	{
		*GameLoadState = 5;
	}
}