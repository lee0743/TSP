#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include "BitmapImage.h"

BitmapImage::BitmapImage()
{
}

BOOL BitmapImage::Load24BitsBitmap(const char * fileName)
{
	BOOL bResult = FALSE;

	FILE* fstream = nullptr;
	DWORD imageSize = 0;
	unsigned char* temp = nullptr;

	fstream = fopen(fileName, "rb");
	if (nullptr == fstream)
	{
		goto bReturn;
	}

	if (mpRawImage != nullptr)
	{
		delete[] mpRawImage;
		mpRawImage = nullptr;
	}

	BitmapFileHeader fileHeader;
	memset(&fileHeader, 0, BITMAP_FILE_HEADER_SIZE);
	
	fread(&fileHeader, BITMAP_FILE_HEADER_SIZE, 1, fstream);

	BitmapInfoHeader infoHeader;
	memset(&infoHeader, 0, BITMAP_INFO_HEADER_SIZE);

	fread(&infoHeader, BITMAP_INFO_HEADER_SIZE, 1, fstream);

	imageSize = infoHeader.ImageSize;

	temp = (unsigned char*)malloc(imageSize * sizeof(unsigned char));
	memset(temp, 0, imageSize);

	fread(temp, imageSize, 1, fstream);

	free(temp);

	fclose(fstream);

	bResult = TRUE;

bReturn:
	return bResult;
}
