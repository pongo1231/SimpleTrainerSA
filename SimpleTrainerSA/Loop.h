#pragma once

class Loop
{
public:
	static Loop* GetInstance();

private:
	Loop() {}

public:
	bool Loop_Invincibility = false;

	void Tick();
};