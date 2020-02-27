#pragma once

#include <d3d11.h>
#include <wrl.h>

class Mesh
{
public:
	Mesh(Microsoft::WRL::ComPtr <ID3D11Buffer> vertexBuffer,
		Microsoft::WRL::ComPtr <ID3D11Buffer> indexBuffer);
	~Mesh();

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
};