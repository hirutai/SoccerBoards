#include "Camera.h"

using namespace DirectX;

void Camera::Initialize()
{
	const float distance = 30.0f;

	eye = { 0, distance, -distance }; // ���_���W
	target = { 0, 0, 0 }; // �����_���W
	up = { 0, 1, 0 }; // ������x�N�g��

	UpdateViewMatrix();

	UpdateProjectionMatrix();
}

void Camera::Update()
{
	UpdateViewMatrix();

	UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix()
{
	// �r���[�s��̌v�Z
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::UpdateProjectionMatrix()
{
	// �������e
	matProjection = 
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(60.0f),
			(float)WindowsApi::windowWidth / WindowsApi::windowHeight,
			0.1f, 1000.0f
		);
}