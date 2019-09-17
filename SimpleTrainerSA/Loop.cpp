#include "Loop.h"
#include "Memory.h"

Loop* _Loop = nullptr;

Loop* Loop::GetInstance()
{
	if (!_Loop)
	{
		_Loop = new Loop();
	}

	return _Loop;
}

void Loop::Tick()
{
	if (Loop_Invincibility)
	{
		Memory::SetPlayerHealth(1000000.f);
	}
	else if (Memory::GetPlayerHealth() == 1000000.f)
	{
		Memory::SetPlayerHealth(100.f);
	}
}