#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <wincodec.h>
#include <string>
#include <memory>

#include "Bitmap.h"


class Loader
{
public:
	Loader();
	~Loader();

	std::unique_ptr<Bitmap> LoadTexture(const std::wstring& filename);

private:
	IWICImagingFactory* m_wicFactory;

};