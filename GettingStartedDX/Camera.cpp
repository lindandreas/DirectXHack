#include "Camera.h"
#include "Utility.h"

Camera::Camera(float fov, float aspect, float zNear, float zFar)
	: m_fov(fov)
	, m_aspect(aspect)
	, m_zNear(zNear)
	, m_zFar(zFar)
{
	ResolveCamera();
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	DirectX::XMFLOAT4X4 m;
	DirectX::XMStoreFloat4x4(&m, m_view);
	return m;
}

DirectX::XMFLOAT4X4 Camera::GetProjMatrix()
{
	DirectX::XMFLOAT4X4 m;
	DirectX::XMStoreFloat4x4(&m, m_proj);
	return m;
}

float Camera::GetZNear()
{
	return m_zNear;
}

float Camera::GetZFar()
{
	return m_zFar;
}

float Camera::GetFov()
{
	return m_fov;
}

float Camera::GetAspect()
{
	return m_aspect;
}

void Camera::Resolve() 
{
	Transform::Resolve();

	ResolveCamera();
}

void Camera::ResolveCamera()
{
	UpdateProj();
	UpdateView();
}

void Camera::UpdateProj()
{
	m_proj = DirectX::XMMatrixPerspectiveFovLH(m_fov, m_aspect, m_zNear, m_zFar);
}

void Camera::UpdateView()
{
	auto world = this->GetWorldMatrix();

	auto forward = DirectX::XMFLOAT3(-world._31, -world._32, -world._33);

	//auto target = this->GetPosition();
	//target.x += forward.x;
	//target.y += forward.y;
	//target.z += forward.z;
	//target = DirectX::XMFLOAT3(0, 0, );

	auto up = DirectX::XMFLOAT3(0, 1, 0);

	DirectX::XMVECTOR xmVecEye = DirectX::XMLoadFloat3(&this->GetPosition());
	DirectX::XMVECTOR xmVecTarget = DirectX::XMLoadFloat3(&forward);
	DirectX::XMVECTOR xmnVecUp = DirectX::XMLoadFloat3(&up);

	m_view = DirectX::XMMatrixLookAtLH(xmVecEye, xmVecTarget, xmnVecUp);
}
