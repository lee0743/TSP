#pragma once

#include <Windows.h>

#pragma pack(push, 1)
struct BitmapFileHeader
{
	WORD Type;
	DWORD FileSize;
	WORD Reserved1;
	WORD Reserved2;
	DWORD Offset;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BitmapInfoHeader
{
	DWORD Size;
	DWORD Width;
	DWORD Height;
	WORD NumPlanes;
	WORD BitsPerPixel;
	DWORD Compression;
	DWORD ImageSize;
	DWORD HorizontalResolution;
	DWORD VerticalResolution;
	DWORD NumColor;
	DWORD NumColorUsed;
};
#pragma pack(pop)

#define BITMAP_FILE_HEADER_SIZE (sizeof(BitmapFileHeader))
#define BITMAP_INFO_HEADER_SIZE (sizeof(BitmapInfoHeader))

class BitmapImage
{
public:
	BitmapImage();
	BOOL Load24BitsBitmap(const char * fileName);
private:
	DWORD mWidth = 0;
	DWORD mHeight = 0;
	char* mpRawImage = nullptr;
};

