#include "Shader.h"
#include <d3dcompiler.h>
#include <iostream>

const std::string Shader::VS_ENTRY_POINT = "";

Shader::Shader()
{

}

std::unique_ptr<Shader> Shader::CreateShader(ID3D11Device& device, std::wstring filename) 
{
	const LPCSTR vsEntryPoint = "vs_main";
	const LPCSTR psEntryPoint = "ps_main";
	const LPCSTR vsTarget = "vs_5_0";
	const LPCSTR psTarget = "ps_5_0";

#ifdef _DEBUG 
	const UINT flags = D3DCOMPILE_DEBUG;
#elif
	const UINT = 0;
#endif

	std::wstring fullPath = Utility::RESOURCE_DIRECTORY + filename;

	Microsoft::WRL::ComPtr<ID3DBlob> errorMsgBlob;
	HRESULT hr = 0;

	auto shader = std::make_unique<Shader>();


	Microsoft::WRL::ComPtr<ID3DBlob> vsByteCode;
	Microsoft::WRL::ComPtr<ID3DBlob> psByteCode;

	// Load and Compile Vertex Shader
	hr = D3DCompileFromFile(fullPath.c_str(), NULL, NULL, vsEntryPoint, vsTarget, flags, 0, &vsByteCode, &errorMsgBlob);
	Utility::PrintBlob(errorMsgBlob);
	if (FAILED(hr))
	{
		Utility::PrintBlob(errorMsgBlob);
		Utility::PrintHRESULT(hr);
	}

	hr = device.CreateVertexShader(vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), NULL, &shader->m_vertexShader);
	if (FAILED(hr))
	{
		Utility::Log(L"Failed to create Vertex Shader");
		Utility::PrintHRESULT(hr);
	}

	// Load and Compile Pixel Shader
	hr = D3DCompileFromFile(fullPath.c_str(), NULL, NULL, psEntryPoint, psTarget, flags, 0, &psByteCode, &errorMsgBlob);
	if (FAILED(hr))
	{
		Utility::PrintBlob(errorMsgBlob);
		Utility::PrintHRESULT(hr);
	}
	hr = device.CreatePixelShader(psByteCode->GetBufferPointer(), psByteCode->GetBufferSize(), NULL, &shader->m_pixelShader);
	if (FAILED(hr))
	{
		Utility::Log(L"Failed to create Pixel Shader");
		Utility::PrintHRESULT(hr);
	}

	// This format must align with the VS_Input struct in basic.hlsl or any other
	// shader you might use.
	// Suggestion: Construct inputElements in runtime by extracting 
	// and handling the code in specified shader.
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		//  D3D11_APPEND_ALIGNED_ELEMENT
	};
	
	hr = device.CreateInputLayout(
		inputElementDesc, 
		ARRAYSIZE(inputElementDesc),
		vsByteCode->GetBufferPointer(), 
		vsByteCode->GetBufferSize(), 
		&shader->m_inputLayout
	);
	if (FAILED(hr))
	{
		Utility::PrintHRESULT(hr);
	}

	Utility::Log(L"Loaded Shader: " + filename);
	return std::move(shader);
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> Shader::GetVertexShader()
{
	return m_vertexShader;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> Shader::GetPixelShader()
{
	return m_pixelShader;
}

Microsoft::WRL::ComPtr<ID3D11InputLayout> Shader::GetInputLayout()
{
	return m_inputLayout;
}