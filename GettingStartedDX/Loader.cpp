#include "Loader.h"
#include "Utility.h"

Loader::Loader()
{
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory1,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_wicFactory)
	);
	if (SUCCEEDED(hr))
	{
		Utility::Log(L"WICImagingFactory Created");
	}



};

Loader::~Loader()
{

}

std::unique_ptr<Bitmap> Loader::LoadTexture(const std::wstring& filename)
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;

	hr = m_wicFactory->CreateDecoderFromFilename(
		filename.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand, // OnLoad?
		&decoder
	);
	if (FAILED(hr))
	{
		Utility::PrintHRESULT(hr);
	}
	hr = m_wicFactory->CreateFormatConverter(&converter);

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	IWICBitmapSource* source;

	hr = decoder->GetFrame(0, &frame);

	//Bitmap* bitmap = new Bitmap(frame);
	auto bitmap = std::make_unique<Bitmap>(frame);
	Utility::Log(L"Bitmap Loaded " + filename);

	
	return std::move(bitmap);
}