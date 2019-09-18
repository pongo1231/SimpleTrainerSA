#pragma once
#include <Windows.h>
#include <d3d9.h>

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
	static inline DWORD PlayerPedBaseAddr()
	{
		static DWORD addr = 0;
		if (!addr)
		{
			addr = FindPattern("\xE8\x00\x00\x00\x00\x2B\xD8", "x????xx");
			addr += 5 + *(DWORD*)(addr + 1);
		}

		return ((int(__cdecl*)(int))addr)(-1);
	}
	static inline float* PlayerHealthAddr()
	{
		return (float*)(PlayerPedBaseAddr() + 1344);
	}
	static inline bool IsReady()
	{
		return _Ready;
	}
	static inline float* PlayerMaxHealthAddr()
	{
		static DWORD addr = 0;
		if (!addr)
		{
			addr = FindPattern("\xF3\xAB\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xE8", "xxxx????????x");
			addr = *(DWORD*)(addr + 4);
		}

		return (float*)addr;
	}
	static inline float* PlayerArmorAddr()
	{
		static DWORD addr = 0;
		if (!addr)
		{
			addr = FindPattern("\xE8\x00\x00\x00\x00\x83\xC4\x18\x8B\x74\x24\x18", "x????xxxxxxx");
			addr += 5 + *(DWORD*)(addr + 1);
			addr = *(DWORD*)(addr + 0x25 + 2);
			addr = *(DWORD*)(addr + 100 * **(BYTE**)(FindPattern("\x8A\x1D\x00\x00\x00\x00\x33\xD2", "xx????xx") + 2)) + 1352;
		}

		return (float*)addr;
	}
	static inline int* PlayerWantedLevelAddr()
	{
		static DWORD addr = 0;
		if (!addr)
		{
			addr = FindPattern("\xE8\x00\x00\x00\x00\x8A\x50\x18", "x????xxx");
			addr += 5 + *(DWORD*)(addr + 1);
		}

		return (int*)(((int(__cdecl*)(int))addr)(-1) + 44);
	}

private:
	static IDirect3D9* _D3d9;
	static IDirect3DDevice9* _D3d9Device;
	static HWND* _Hwnd;
	static bool _Ready;

	static HRESULT __stdcall Hook_BeginScene(IDirect3DDevice9* device);
	static HRESULT __stdcall Hook_EndScene(IDirect3DDevice9* device);
	static HRESULT __stdcall Hook_Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
	static int __stdcall Hook_WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static void _SkipIntroSequence();

private:
	Memory() {}
};