#include "Main.h"
#include "Drawing.h"
#include "Memory.h"

DWORD __stdcall Script::Main(LPVOID nothing)
{
	Memory::Init();

	Drawing::DrawText("Press - to open Simple Trainer", .05f, .05f, 30, 20, { 255, 255, 60, 60 }, 300);

	return 0;
}