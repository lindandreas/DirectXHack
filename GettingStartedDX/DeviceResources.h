#pragma once
#include <d3d11.h>
#include <wrl.h>

class DeviceResources
{
public:
	DeviceResources(HWND hWnd);
	~DeviceResources();

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext();
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain();

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRenderTargetView();
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView();



private:
	HRESULT InitDeviceAndContext();
	HRESULT InitSwapChain(HWND hWnd);


	Microsoft::WRL::ComPtr<ID3D11Device> m_pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pd3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pDXGISwapChain;

	// BackBuffer & View
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;

	// DepthStencil & View
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	D3D_FEATURE_LEVEL featureLevel;
	D3D11_TEXTURE2D_DESC m_backbufferDesc;

};