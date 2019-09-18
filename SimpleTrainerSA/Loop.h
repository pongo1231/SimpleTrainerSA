#pragma once

class Loop
{
public:
	static Loop* GetInstance();

private:
	Loop() {}

public:
	bool Loop_Invincibility = false;
	bool Loop_ShowFPS = false;

	void Tick();
};