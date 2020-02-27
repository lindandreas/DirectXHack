#include <iostream>


#include  <math.h>

#include "Renderer.h"
#include "Utility.h"
#include "Color.h"
#include "Vertex.h"

// Intellisense did not like #define _USE_MATH_DEFINES,
// So, here it is instead.
#define M_PI 3.14159265358979323846

Renderer::Renderer(
	SceneResources& sceneResources,
	DeviceResources& deviceResources,
	int renderResX, int renderResY,
	int targetFPS
)
	: m_sceneResources(sceneResources)
	, m_deviceResources(deviceResources)
	, m_windowHeight(renderResX)
	, m_windowWidth(renderResY)
{
	auto device = m_deviceResources.GetDevice();
	auto context = m_deviceResources.GetDeviceContext();

	m_camera = std::make_unique<Camera>(M_PI / 2,  (float)m_windowHeight / (float)m_windowWidth, 0.1f, 100.0f);
	m_camera->SetPosition(0, 0, -3);


	m_sceneResources.testTransform.SetPosition(0, 0, 0);
	m_sceneResources.testTransform.SetEulerRotation(0, 0, 0);
	InitConstantBuffer();

	// Create Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Height = (FLOAT)renderResY;
	viewport.Width = (FLOAT)renderResX;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	context->RSSetViewports(1, &viewport);

	// Define settings for Rasterizer
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	desc.FrontCounterClockwise = FALSE;				   
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> state;
	HRESULT hr = device->CreateRasterizerState(&desc, &state);
	if (FAILED(hr))
	{
		Utility::PrintHRESULT(hr);
	}
	context->RSSetState(state.Get());


	// Set Primitive Topology
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set Render Targets
	auto rtv = m_deviceResources.GetRenderTargetView();
	auto dsv = m_deviceResources.GetDepthStencilView();

	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
}

Renderer::~Renderer()
{
	// Check Renderer.h on why this is released manually
	// tldr: too lazy to debug
	m_constantBuffer->Release();
}

void Renderer::Draw()
{
	// Update the rotation of the transform used for the mesh
	// This is a bad way to do it as it binds the "game time" to the
	// framerate. A good way to do this is measuring the time between 
	// the last frame and this frame and multiply things like
	// movement, rotation and such with this new deltaTime.
	auto rotation = m_sceneResources.testTransform.GetEulerRotation();
	rotation.x += 0.01;
	rotation.y += 0.01;
	rotation.z += 0.01;
	m_sceneResources.testTransform.SetEulerRotation(rotation);

	// Fetch DeviceResources
	auto device = m_deviceResources.GetDevice();
	auto context = m_deviceResources.GetDeviceContext();

	// Fetch RenderTargetViews
	auto rtv = m_deviceResources.GetRenderTargetView();
	auto dsv = m_deviceResources.GetDepthStencilView();

	// Set BackBuffer RenderTargetView and DepthBuffer RenderTargetView in context
	// 
	// This should not be done every frame
	// But for some reason it wont play nice and behaves wierd if I dont
	context->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());

	// Clear Depth and Color
	// DepthBuffer is cleared to 1.0f as that is the furthest distance.
	FLOAT color[] = { .4f, .4f, .8f, 1.0f };
	context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->ClearRenderTargetView(rtv.Get(), color);

	// Set Shaders to context
	context->VSSetShader(m_sceneResources.shader->GetVertexShader().Get(), NULL, 0);
	UpdateConstantBuffer();
	context->PSSetShader(m_sceneResources.shader->GetPixelShader().Get(), NULL, 0);

	// Set InputLayout for the Vertex Shader to context
	context->IASetInputLayout(*m_sceneResources.shader->GetInputLayout().GetAddressOf());

	// Set Vertex and Index Buffer to context
	Mesh* mesh = m_sceneResources.mesh.get();
	auto indexBuffer = m_sceneResources.mesh->GetIndexBuffer();
	auto vertexBuffer = m_sceneResources.mesh->GetVertexBuffer();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	// Draw the three vertices in the Vertex buffer, starting from index 0
	// If you have an index buffer as well, use DrawIndexed instead.
	context->DrawIndexed(36, 0, 0);

	// Finally, present the render target!
	m_deviceResources.GetSwapChain()->Present(1, 0);
}

// Initializes the constant buffer
// Note:
// You can have multiple constant buffers ex.
// One with data that are updated once per frame
// One with data that are updated once per draw
void Renderer::InitConstantBuffer()
{
	// From the Transform we can get the model/world matrix
	// This should be switched out to one transform per object
	Transform transform = m_sceneResources.testTransform;

	// Initialize Data for the ConstantBuffer
	VS_CONSTANT_BUFFER initialData;
	initialData.MODEL = transform.GetWorldMatrix();
	initialData.VIEW = m_camera->GetViewMatrix();
	initialData.PROJ = m_camera->GetProjMatrix();

	// Create Description for the ConstantBuffer
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	// Initialice SUBRESOURCE_DATA that points to initialData
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &initialData;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// Create the buffer.
	HRESULT hr = m_deviceResources.GetDevice()->CreateBuffer(
		&desc, 
		&data, 
		&m_constantBuffer
	);

	if (FAILED(hr))
	{
		Utility::PrintHRESULT(L"Failed to Create ConstantBuffer", hr);
	}

	m_deviceResources.GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_constantBuffer);
}


// Updated the constant buffer each frame
void Renderer::UpdateConstantBuffer()
{
	Transform transform = m_sceneResources.testTransform;

	VS_CONSTANT_BUFFER initialData;
	initialData.MODEL = transform.GetWorldMatrix();
	initialData.VIEW = m_camera->GetViewMatrix();
	initialData.PROJ = m_camera->GetProjMatrix();

	m_deviceResources.GetDeviceContext()->UpdateSubresource(
		m_constantBuffer, 
		0, 
		nullptr, 
		&initialData, 
		0, 
		0
	);
}