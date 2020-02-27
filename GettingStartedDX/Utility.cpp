#include "Utility.h"
#include <comdef.h>
#include <iostream>
#include <d3dcompiler.h>



const std::wstring Utility::RESOURCE_DIRECTORY = L"../resources/";

void Utility::PrintHRESULT(HRESULT hresult)
{
	_com_error err(hresult);
	std::wcout << "0x" << std::hex << hresult << " - " << err.ErrorMessage() << std::endl;
}

void Utility::PrintHRESULT(std::wstring str, HRESULT hresult)
{
	_com_error err(hresult);
	std::wcout << str << " - 0x" << std::hex << hresult << " - " << err.ErrorMessage() << std::endl;
}

void Utility::Log(std::wstring msg)
{
	std::wcout << msg << std::endl;
}

void Utility::PrintBlob(Microsoft::WRL::ComPtr<ID3DBlob> blob)
{
	if (blob != NULL)
	{
		SIZE_T size = blob->GetBufferSize();
		char* ptr = (char*)blob->GetBufferPointer();
		std::printf(ptr);
		std::string str;
	}
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Utility::CreateTri(ID3D11Device& device)
{
	Vertex vertexData[] =
	{
		{ DirectX::XMFLOAT3(0,   .5f, 0), DirectX::XMFLOAT3(1, 0, 0) },
		{ DirectX::XMFLOAT3(.5f, -.5f, 0), DirectX::XMFLOAT3(0, 1, 0) },
		{ DirectX::XMFLOAT3(-.5f, -.5f, 0), DirectX::XMFLOAT3(0, 0, 1) }
	};

	CD3D11_BUFFER_DESC vBufferDesc = CD3D11_BUFFER_DESC(
		sizeof(vertexData),
		D3D11_BIND_VERTEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA vData;
	ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));
	vData.pSysMem = vertexData;
	vData.SysMemPitch = 0;
	vData.SysMemSlicePitch = 0;


	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	HRESULT hr = device.CreateBuffer(&vBufferDesc, &vData, &buffer);

	if (FAILED(hr))
	{
		Utility::PrintHRESULT(hr);
	}

	return buffer;
}

std::unique_ptr<Mesh> Utility::CreateTriWithIndex(ID3D11Device& device)
{
	Vertex vertexData[] =
	{
		{ DirectX::XMFLOAT3(0,   .5f, 0), DirectX::XMFLOAT3(1, 0, 0) },
		{ DirectX::XMFLOAT3(.5f, -.5f, 0), DirectX::XMFLOAT3(0, 1, 0) },
		{ DirectX::XMFLOAT3(-.5f, -.5f, 0), DirectX::XMFLOAT3(0, 0, 1) }
	};

	UINT indexData[] =
	{
		0, 1, 2
	};

	auto vBuffer = CreateVertexBuffer(device, vertexData, sizeof(vertexData));
	auto iBuffer = CreateIndexBuffer(device, indexData, sizeof(indexData));

	return std::move(std::make_unique<Mesh>(vBuffer, iBuffer));
}

std::unique_ptr<Mesh> Utility::CreateSquare(ID3D11Device& device)
{
	Vertex vertexData[] =
	{
		{ DirectX::XMFLOAT3(-1,-1,-1), DirectX::XMFLOAT3(1, 0, 0) },
		{ DirectX::XMFLOAT3(1,-1,-1), DirectX::XMFLOAT3(0, 1, 0) },
		{ DirectX::XMFLOAT3(-1, 1,-1), DirectX::XMFLOAT3(0, 0, 1) },
		{ DirectX::XMFLOAT3(1, 1,-1), DirectX::XMFLOAT3(1, 1, 0) },
		{ DirectX::XMFLOAT3(-1,-1, 1), DirectX::XMFLOAT3(1, 0, 1) },
		{ DirectX::XMFLOAT3(1,-1, 1), DirectX::XMFLOAT3(1, 1, 1) },
		{ DirectX::XMFLOAT3(-1, 1, 1), DirectX::XMFLOAT3(1, 0, 1) },
		{ DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0) },
	};

	UINT indexData[] =
	{
		0, 1, 2, // front
		3, 2, 1,
		0, 4, 1, // top
		4, 5, 1,
		2, 3, 6, // bottom
		6, 7, 3,
		4, 6, 2, // left side
		0, 2, 4,
		7, 1, 3, // right side
		1, 5, 7,
		5, 4, 7, // back 
		7, 6, 4
	};

	auto vBuffer = CreateVertexBuffer(device, vertexData, sizeof(vertexData));
	auto iBuffer = CreateIndexBuffer(device, indexData, sizeof(indexData));

	return std::move(std::make_unique<Mesh>(vBuffer, iBuffer));
}

std::unique_ptr<Mesh> Utility::CreateSphere(ID3D11Device& device, int resX, int resY)
{
	return nullptr;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Utility::CreateVertexBuffer(ID3D11Device& device, Vertex* verticies, size_t arraySize)
{
	CD3D11_BUFFER_DESC vBufferDesc = CD3D11_BUFFER_DESC(
		arraySize,
		D3D11_BIND_VERTEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = verticies;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	HRESULT hr = device.CreateBuffer(&vBufferDesc, &data, &buffer);

	if (FAILED(hr))
	{
		Utility::PrintHRESULT(L"Failed to create Vertex Buffer", hr);
	}
	return buffer;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Utility::CreateIndexBuffer(ID3D11Device& device, UINT* indicies, size_t arraySize)
{

	CD3D11_BUFFER_DESC iBufferDesc = CD3D11_BUFFER_DESC(
		arraySize,
		D3D11_BIND_INDEX_BUFFER
	);

	D3D11_SUBRESOURCE_DATA iData;
	ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));
	iData.pSysMem = indicies;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	Microsoft::WRL::ComPtr<ID3D11Buffer> iBuffer;
	HRESULT hr = device.CreateBuffer(&iBufferDesc, &iData, &iBuffer);

	if (FAILED(hr))
	{
		Utility::PrintHRESULT(L"Failed to create Index Buffer", hr);
	}
	return iBuffer;
}