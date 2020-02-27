#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <string>
#include "Utility.h"

class Shader
{
public:
	Shader();
	static std::unique_ptr<Shader> CreateShader(ID3D11Device& device, std::wstring filename);

	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout();

private:
	static const std::string VS_ENTRY_POINT;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};