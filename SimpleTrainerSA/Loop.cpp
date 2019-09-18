#include "Loop.h"
#include "Memory.h"
#include "Drawing.h"
#include <time.h>
#include <sstream>

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
		*Memory::PlayerHealthAddr() = 1000000.f;
	}
	else if (*Memory::PlayerHealthAddr() == 1000000.f)
	{
		*Memory::PlayerHealthAddr() = *Memory::PlayerMaxHealthAddr() / 5.f;
	}

	if (Loop_ShowFPS)
	{
		static DWORD lastFrames = 0;
		static DWORD framesCounter = 0;
		static DWORD pastTime = time(0);

		framesCounter++;

		if (difftime(time(0), pastTime) >= 1.)
		{
			pastTime = time(0);
			lastFrames = framesCounter;
			framesCounter = 0;
		}

		std::ostringstream oss;
		oss << "FPS: " << lastFrames;
		Drawing::DrawTextA(oss.str(), .6f, .025f, 15, 10, { 255, 180, 180, 180 });
	}
}