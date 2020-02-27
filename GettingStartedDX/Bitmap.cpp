#include "Bitmap.h"


Bitmap::Bitmap(Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> wicBitmapFrame)
{
	wicBitmapFrame->GetSize(&m_width, &m_height);

	int bufferSize = m_width * m_height;
	m_pixelBuffer = new BYTE[bufferSize];

	wicBitmapFrame->CopyPixels(
		NULL,			// NULL = Whole Image
		4,				// 4 BYTE = RGBA
		bufferSize,
		m_pixelBuffer
	);
}

Bitmap::~Bitmap()
{
	delete[] m_pixelBuffer;
}


UINT Bitmap::GetWidth()
{
	return m_width;
}

UINT Bitmap::GetHeight()
{
	return m_height;
}

BYTE* Bitmap::GetPixelBuffer()
{
	return m_pixelBuffer;
}