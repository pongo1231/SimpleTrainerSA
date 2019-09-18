#include "Main.h"
#include "Drawing.h"
#include "Memory.h"

DWORD __stdcall Script::Main(LPVOID nothing)
{
	Memory::Init();

	Drawing::DrawText("Press - to open Simple Trainer", .025f, .025f, 25, 15, { 255, 255, 60, 60 }, 300);

	return 0;
}