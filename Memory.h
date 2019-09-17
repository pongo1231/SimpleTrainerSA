#pragma once
#include <Windows.h>

struct IDirect3D9;
struct IDirect3DDevice9;

class Memory
{
public:
	static DWORD Base;
	static DWORD End;

	static void Init();
	static DWORD FindPattern(const char* pattern, const char* mask);

	static inline IDirect3D9* GetD3D9Interface()
	{
		return _D3d9;
	}
	static inline IDirect3DDevice9* GetD3D9Device()
	{
		return _D3d9Device;
	}
	static inline HWND* GetHwnd()
	{
		return _Hwnd;
	}
	static inline DWORD GetPlayerPedBaseAddr()
	{
		static DWORD addr = 0;
		if (!addr)
		{
			/*addr = FindPattern("\xE8\x00\x00\x00\x00\xD9\x47\x20", "x????xxx");
			addr += 5 + *(DWORD*)(addr + 1);*/

			addr = FindPattern("\xE8\x00\x00\x00\x00\x2B\xD8", "x????xx");
			addr += 5 + *(DWORD*)(addr + 1);
		}

		return ((int(__cdecl*)(int))addr)(-1);
	}

private:
	static IDirect3D9* _D3d9;
	static IDirect3DDevice9* _D3d9Device;
	static HWND* _Hwnd;

	static void _SkipIntroSequence();

private:
	Memory() {}
};