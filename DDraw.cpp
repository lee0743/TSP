#include "DDraw.h"

DDraw::DDraw()
{
}

BOOL DDraw::Initialize()
{
	BOOL bResult = FALSE;

	DDSURFACEDESC2 ddsd = {};
	DWORD width;
	DWORD height;

	if (DD_OK != DirectDrawCreate(nullptr, &mpDD, nullptr))
	{
		MessageBox(mHWnd, L"DirectDrawCreate failed!", nullptr, MB_OK);
		goto bReturn;
	}

	if (DD_OK != mpDD->QueryInterface(IID_IDirectDraw7, (LPVOID*)&mpDD7))
	{
		MessageBox(mHWnd, L"QueryInterface failed!", nullptr, MB_OK);
		goto bReturn;
	}

	if (DD_OK != mpDD7->SetCooperativeLevel(mHWnd, DDSCL_NORMAL))
	{
		MessageBox(mHWnd, L"SetCooperativeLevel failed!", nullptr, MB_OK);
		goto bReturn;
	}

	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if (DD_OK != mpDD7->CreateSurface(&ddsd, &mpDDPrimary, nullptr))
	{
		MessageBox(mHWnd, L"CreateSurface failed!", nullptr, MB_OK);
		goto bReturn;
	}
	
	if (DD_OK != mpDD7->CreateClipper(0, &mpClipper, nullptr))
	{
		MessageBox(mHWnd, L"CreateSurface failed!", nullptr, MB_OK);
		goto bReturn;
	}

	mpClipper->SetHWnd(0, mHWnd);
	mpDDPrimary->SetClipper(mpClipper);

	GetClientRect(mHWnd, &mWindow);
	::ClientToScreen(mHWnd, (POINT*)(mWindow.left));
	::ClientToScreen(mHWnd, (POINT*)(mWindow.right));

	width = mWindow.right - mWindow.left;
	height = mWindow.bottom - mWindow.top;

	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;

	if (DD_OK != mpDD7->CreateSurface(&ddsd, &mpDDBack, nullptr))
	{
		MessageBox(mHWnd, L"CreateSurface failed!", nullptr, MB_OK);
		goto bReturn;
	}

	bResult = TRUE;

bReturn:
	return bResult;
}
