#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXMathMatrix.inl>
#include <wrl.h>
#include "DeviceResources.h"
#include "SceneResources.h"
#include "Shader.h"
#include "Transform.h"
#include "Camera.h"

class Renderer
{
public:

	// This buffer must match cbuffer in shader
	// Packing Rules for Constant Variables reference
	// https://docs.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
	// 64 bytes x 4
	struct VS_CONSTANT_BUFFER
	{
		DirectX::XMFLOAT4X4 MODEL;
		DirectX::XMFLOAT4X4 VIEW;
		DirectX::XMFLOAT4X4 PROJ;
	};

	Renderer(
		SceneResources& sceneResources, 
		DeviceResources& deviceResources, 
		int renderResX, int renderResY, 
		int targetFPS
	);
	~Renderer();

	void Draw();


private:
	void InitConstantBuffer();
	void UpdateConstantBuffer();
	
	int m_windowWidth;
	int m_windowHeight;

	DeviceResources& m_deviceResources;
	SceneResources& m_sceneResources;

	// For some reason, when using a ComPtr and running VSSetConstantBuffer 
	// m_constantBuffer becomes NULL
	// I'm to lazy to look in to this, so I'll use a raw pointer 
	ID3D11Buffer* m_constantBuffer;

	std::unique_ptr<Camera> m_camera;
};
