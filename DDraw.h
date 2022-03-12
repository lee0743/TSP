#pragma once

#include <ddraw.h>

class DDraw
{
public:
	DDraw();
	BOOL Initialize();
private:
	IDirectDraw* mpDD = nullptr;
	IDirectDraw7* mpDD7 = nullptr;
	IDirectDrawSurface7* mpDDPrimary = nullptr;
	IDirectDrawSurface7* mpDDBack = nullptr;
	IDirectDrawClipper* mpClipper = nullptr;

	HWND mHWnd;
	RECT mWindow;
};

