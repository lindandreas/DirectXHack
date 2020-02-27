#pragma once
#include <winerror.h>
#include <string>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <memory>

#include "Vertex.h"
#include "Mesh.h"

class Utility
{
private:
	Utility();
public:
	static void PrintHRESULT(HRESULT hresult);
	static void PrintHRESULT(std::wstring str, HRESULT hresult);
	static void Log(std::wstring msg);

	static const std::wstring RESOURCE_DIRECTORY;

	static void PrintBlob(Microsoft::WRL::ComPtr<ID3DBlob> blob);


	static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateTri(ID3D11Device& device);
	static std::unique_ptr<Mesh> CreateTriWithIndex(ID3D11Device& device);

	static std::unique_ptr<Mesh> CreateSquare(ID3D11Device& device);
	static std::unique_ptr<Mesh> CreateSphere(ID3D11Device& device, int resX, int resY);
private:
	static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device& device, Vertex* verticies, size_t arraySize);
	static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateIndexBuffer(ID3D11Device& device, UINT* indicies, size_t arraySize);
};