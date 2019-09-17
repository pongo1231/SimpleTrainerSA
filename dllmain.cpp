#include <Windows.h>
#include <Psapi.h>
#include "main.h"
#include "memory.h"

BOOL WINAPI DllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	static HANDLE thread;
	MODULEINFO moduleInfo{ 0 };

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(moduleInfo));
		Memory::Base = (DWORD) moduleInfo.lpBaseOfDll;
		Memory::End = Memory::Base + moduleInfo.SizeOfImage;
		thread = CreateThread(NULL, 0, Script::Main, NULL, 0, NULL);
		break;
	case DLL_PROCESS_DETACH:
		CloseHandle(&thread);
		break;
	}

	return TRUE;
}