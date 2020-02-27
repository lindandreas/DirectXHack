#include "Transform.h"
#include "Utility.h"


Transform::Transform()
{
	m_xmF3Position = DirectX::XMFLOAT3(0, 0, 0);
	m_xmMatTranslation = DirectX::XMMatrixTranslation(0, 0, 0);

	m_xmF3Euler = DirectX::XMFLOAT3(0, 0, 0);
	m_xmMatRotation = DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0);

	m_xmF3Scale = DirectX::XMFLOAT3(1, 1, 1);
	m_xmMatScale = DirectX::XMMatrixScaling(1, 1, 1);

	Resolve();
}

Transform::~Transform()
{
	
}

void Transform::SetPosition(float x, float y, float z)
{
	m_xmF3Position = DirectX::XMFLOAT3(x, y, z);
	m_xmMatTranslation = DirectX::XMMatrixTranslation(x, y, z);
	Resolve();
}
void Transform::SetEulerRotation(float x, float y, float z)
{
	m_xmF3Euler = DirectX::XMFLOAT3(x, y, z);
	m_xmMatRotation = DirectX::XMMatrixRotationRollPitchYaw(x, y, z);
	Resolve();
}
void Transform::SetScale(float x, float y, float z)
{
	m_xmF3Scale = DirectX::XMFLOAT3(x, y, z);
	m_xmMatScale = DirectX::XMMatrixScaling(x, y, z);
	Resolve();
}

void Transform::SetPosition(DirectX::XMFLOAT3 position)
{
	SetPosition(position.x, position.y, position.z);
}
void Transform::SetEulerRotation(DirectX::XMFLOAT3 euler)
{
	SetEulerRotation(euler.x, euler.y, euler.z);
}
void Transform::SetScale(DirectX::XMFLOAT3 scale)
{
	SetScale(scale.x, scale.y, scale.z);
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return m_xmF3Position;
}
DirectX::XMFLOAT3 Transform::GetEulerRotation()
{
	return m_xmF3Euler;
}
DirectX::XMFLOAT3 Transform::GetScale()
{
	return m_xmF3Scale;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	DirectX::XMFLOAT4X4 m;
	DirectX::XMStoreFloat4x4(&m, m_xmMatWorld);
	return m;
}

void Transform::Resolve()
{
	m_xmMatWorld = (m_xmMatScale * m_xmMatRotation) * m_xmMatTranslation;
}