#include "Mesh.h"

Mesh::Mesh(Microsoft::WRL::ComPtr <ID3D11Buffer> vertexBuffer,
	Microsoft::WRL::ComPtr <ID3D11Buffer> indexBuffer)
	: m_vertexBuffer(vertexBuffer)
	, m_indexBuffer(indexBuffer)
{
	
}
Mesh::~Mesh()
{

}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer()
{
	return m_vertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer()
{
	return m_indexBuffer;
}