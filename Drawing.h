#pragma once
#include "Memory.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

#define MAX_TEXTS 16

struct ARGB
{
	ARGB(UCHAR a, UCHAR r, UCHAR g, UCHAR b) : _A(a), _R(r), _G(g), _B(b) {}

	inline UCHAR GetA() const
	{
		return _A;
	}
	inline UCHAR GetR() const
	{
		return _R;
	}
	inline UCHAR GetG() const
	{
		return _G;
	}
	inline UCHAR GetB() const
	{
		return _B;
	}

private:
	UCHAR _A;
	UCHAR _R;
	UCHAR _G;
	UCHAR _B;
};

class Drawing
{
public:
	static void NewFrame();
	static void DrawText(std::string label, float x, float y, UINT height, UINT width, ARGB color, UINT frames = 1);
	static void DrawText(std::string label, float x, float y, LPD3DXFONT font, ARGB color, UINT frames = 1);

private:
	Drawing() {}

private:
	struct Text
	{
		Text(std::string label, float x, float y, UINT height, UINT width, ARGB color, UINT frames) : _Label(label), _X(x), _Y(y),
			_Color(color), Frames(frames), _CachedFont(nullptr), _CustomFont(false)
		{
			D3DXCreateFont(Memory::GetD3D9Device(), height, width, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE, "Arial", &_CachedFont);
		}

		Text(std::string label, float x, float y, LPD3DXFONT font, ARGB color, UINT frames) : _Label(label), _X(x), _Y(y),
			_CachedFont(font), _Color(color), Frames(frames), _CustomFont(true) {}

		~Text()
		{
			if (!_CustomFont)
			{
				_CachedFont->Release();
			}
		}

		UINT Frames;

		inline std::string GetLabel() const
		{
			return _Label;
		}
		inline float GetX() const
		{
			return _X;
		}
		inline float GetY() const
		{
			return _Y;
		}
		inline ARGB GetColor() const
		{
			return _Color;
		}
		inline LPD3DXFONT GetCachedFont() const
		{
			return _CachedFont;
		}

	private:
		std::string _Label;
		float _X;
		float _Y;
		ARGB _Color;
		LPD3DXFONT _CachedFont;
		bool _CustomFont;
	};

	static Text* _ToDrawTexts[MAX_TEXTS];
};