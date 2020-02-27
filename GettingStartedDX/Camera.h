#pragma once
#include "Transform.h"

class Camera : public Transform
{
public:
	Camera(float fov, float aspect, float zNear, float zFar);

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjMatrix();
	
	float GetZNear();
	float GetZFar();
	float GetFov();
	float GetAspect();

protected:
	void Resolve() override;

private:
	void ResolveCamera();

	void UpdateProj();
	void UpdateView();

	float m_zNear; 
	float m_zFar; 
	float m_fov;
	float m_aspect;

	DirectX::XMMATRIX m_view;
	DirectX::XMMATRIX m_proj;

};