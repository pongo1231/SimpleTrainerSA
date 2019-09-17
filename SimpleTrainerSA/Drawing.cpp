#include "Drawing.h"
#include "Memory.h"
#include "Menus/Menu.h"
#include "Logging.h"
#include <d3d9.h>
#include <d3dx9.h>

Drawing::Text* Drawing::_ToDrawTexts[MAX_TEXTS] = { nullptr };

void Drawing::NewFrame()
{
	auto device = Memory::GetD3D9Device();
	D3DVIEWPORT9 viewport;
	device->GetViewport(&viewport);
	auto viewportWidth = viewport.Width;
	auto viewportHeight = viewport.Height;

	Menu* currentMenu = Menu::GetCurrentMenu();
	if (currentMenu)
	{
		currentMenu->Tick();
	}

	for (int i = 0; i < MAX_TEXTS; i++)
	{
		auto*& text = _ToDrawTexts[i];
		if (!text)
		{
			continue;
		}

		RECT rect;
		rect.left = viewportWidth * text->GetX();
		rect.right = viewportWidth;
		rect.top = viewportHeight * text->GetY();
		rect.bottom = viewportHeight;

		auto color = text->GetColor();
		text->GetCachedFont()->DrawTextA(NULL, text->GetLabel().c_str(), -1, &rect, 0, D3DCOLOR_ARGB(color.GetA(), color.GetR(),
			color.GetG(), color.GetB()));

		if (--text->Frames == 0)
		{
			delete text;
			text = nullptr;
		}
	}
}

void Drawing::DrawText(std::string label, float x, float y, UINT height, UINT width, ARGB color, UINT frames)
{
	if (!label.empty() && x >= 0.f && x <= 1.f && y >= 0.f && y <= 1.f && frames > 0)
	{
		for (int i = 0; i < MAX_TEXTS; i++)
		{
			if (!_ToDrawTexts[i])
			{
				_ToDrawTexts[i] = new Text(label, x, y, height, width, color, frames);
				break;
			}
		}
	}
}

void Drawing::DrawText(std::string label, float x, float y, LPD3DXFONT font, ARGB color, UINT frames)
{
	if (!label.empty() && x >= 0.f && x <= 1.f && y >= 0.f && y <= 1.f && frames > 0)
	{
		for (int i = 0; i < MAX_TEXTS; i++)
		{
			if (!_ToDrawTexts[i])
			{
				_ToDrawTexts[i] = new Text(label, x, y, font, color, frames);
				break;
			}
		}
	}
}