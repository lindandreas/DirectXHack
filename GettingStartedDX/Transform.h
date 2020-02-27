#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

class Transform
{
public:
	Transform();
	~Transform();

	void SetPosition(float x, float y, float z);
	void SetEulerRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	void SetPosition(DirectX::XMFLOAT3 position);
	void SetEulerRotation(DirectX::XMFLOAT3 euler);
	void SetScale(DirectX::XMFLOAT3 scale);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetEulerRotation();
	DirectX::XMFLOAT3 GetScale();

	DirectX::XMFLOAT4X4 GetWorldMatrix();

protected:
	virtual void Resolve();
private:
	DirectX::XMFLOAT3 m_xmF3Position;
	DirectX::XMFLOAT3 m_xmF3Euler;
	DirectX::XMFLOAT3 m_xmF3Scale;

	DirectX::XMMATRIX m_xmMatTranslation;
	DirectX::XMMATRIX m_xmMatRotation;
	DirectX::XMMATRIX m_xmMatScale;

	DirectX::XMMATRIX m_xmMatWorld;
};