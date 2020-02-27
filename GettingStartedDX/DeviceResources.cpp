#include "DeviceResources.h"
#include "Utility.h"

DeviceResources::DeviceResources(HWND hWnd)
{
	HRESULT hr;

	hr = InitDeviceAndContext();
	if (FAILED(hr))
		Utility::PrintHRESULT(hr);
	else
		Utility::Log(L"Device Created!");

	hr = InitSwapChain(hWnd);
	if (FAILED(hr))
		Utility::PrintHRESULT(hr);
	else
		Utility::Log(L"SwapChain Created");

	

	// Create RenderTargetView (RTV)
	hr = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &m_pBackBuffer);
	hr = m_pd3dDevice->CreateRenderTargetView(
		m_pBackBuffer.Get(), 
		nullptr, 
		m_pRenderTarget.GetAddressOf());

	m_pBackBuffer->GetDesc(&m_backbufferDesc);

	// Create DepthStencilView
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT>(m_backbufferDesc.Width),
		static_cast<UINT>(m_backbufferDesc.Height),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);

	m_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &m_pDepthStencil);
	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);

	m_pd3dDevice->CreateDepthStencilView(
		m_pDepthStencil.Get(),
		&depthStencilViewDesc,
		&m_pDepthStencilView);
}

DeviceResources::~DeviceResources()
{

}

HRESULT DeviceResources::InitDeviceAndContext()
{
	D3D_FEATURE_LEVEL levels[] = {
		//D3D_FEATURE_LEVEL_9_1,
		//D3D_FEATURE_LEVEL_9_2,
		//D3D_FEATURE_LEVEL_9_3,
		//D3D_FEATURE_LEVEL_10_0,
		//D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		//D3D_FEATURE_LEVEL_11_1
	};
	//auto deviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
	auto deviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL, deviceFlags, levels,
		ARRAYSIZE(levels),
		D3D11_SDK_VERSION,
		&device,
		&featureLevel,
		&context
	);

	device.As(&m_pd3dDevice);
	context.As(&m_pd3dDeviceContext);

	return hr;
}

HRESULT DeviceResources::InitSwapChain(HWND hWnd)
{
	// Create Swapchain
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapDesc.Windowed = TRUE;
	swapDesc.BufferCount = 2;
	//swapDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapDesc.OutputWindow = hWnd;

	Microsoft::WRL::ComPtr<IDXGIDevice1> dxgiDevice;
	m_pd3dDevice.As(&dxgiDevice);

	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> factory;


	HRESULT hr = dxgiDevice->GetAdapter(&adapter);

	if (SUCCEEDED(hr))
	{
		adapter->GetParent(IID_PPV_ARGS(&factory));

		hr = factory->CreateSwapChain(
			m_pd3dDevice.Get(),
			&swapDesc,
			&m_pDXGISwapChain
		);
		if (FAILED(hr))
		{
			Utility::PrintHRESULT(hr);
		}
	}
	else
	{
		Utility::PrintHRESULT(hr);
	}

	return hr;
}



Microsoft::WRL::ComPtr<ID3D11Device> DeviceResources::GetDevice()
{
	return m_pd3dDevice;
}

Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceResources::GetDeviceContext()
{
	return m_pd3dDeviceContext;
}

Microsoft::WRL::ComPtr<IDXGISwapChain> DeviceResources::GetSwapChain()
{
	return m_pDXGISwapChain;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> DeviceResources::GetRenderTargetView()
{
	return m_pRenderTarget;
}

Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DeviceResources::GetDepthStencilView()
{
	return m_pDepthStencilView;
}