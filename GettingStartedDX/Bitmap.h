#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <wincodec.h>
#include <memory>

#include "Utility.h"


// I have not tested using bitmaps in this project.
// Hope it works
//
class Bitmap
{
public:
	Bitmap(Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> wicBitmapFrame);
	~Bitmap();

	UINT GetWidth();
	UINT GetHeight();
	BYTE* GetPixelBuffer();

private:
	UINT m_width;
	UINT m_height;
	BYTE* m_pixelBuffer;
};