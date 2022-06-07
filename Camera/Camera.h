#pragma once
#include <DirectXMath.h>

#include "WindowsApi.h"

/// <summary>
/// �J�����N���X
/// </summary>
class Camera
{
#pragma region �G�C���A�X
public:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �r���[�s��̍X�V
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// �v���W�F�N�V�����̍X�V
	/// </summary>
	void UpdateProjectionMatrix();
#pragma endregion

#pragma region setter
public:
	/// <summary>
	/// �r���[�s��̐ݒ�
	/// </summary>
	/// <param name="matView">�r���[�s��</param>
	void SetMatView(const XMMATRIX& matView) { this->matView = matView; }

	/// <summary>
	/// �v���W�F�N�V�����s��̐ݒ�
	/// </summary>
	/// <param name="matProjection">�v���W�F�N�V�����s��</param>
	void SetMatProjection(const XMMATRIX& matProjection) { this->matProjection = matProjection; }

	/// <summary>
	/// ���_���W�̐ݒ�
	/// </summary>
	/// <param name="eye">���_���W</param>
	void SetEye(const XMFLOAT3& eye) { this->eye = eye; }

	/// <summary>
	/// �����_���W�̐ݒ�
	/// </summary>
	/// <param name="target">�����_���W</param>
	void SetTarget(const XMFLOAT3& target) { this->target = target; }

	/// <summary>
	/// ������x�N�g���̐ݒ�
	/// </summary>
	/// <param name="up">������x�N�g��</param>
	void SetUp(const XMFLOAT3& up) { this->up = up; }
#pragma endregion

#pragma region getter
public:
	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns></returns>
	const XMMATRIX& GetMatView() { return matView; }

	/// <summary>
	/// �v���W�F�N�V�����s��̎擾
	/// </summary>
	/// <returns></returns>
	const XMMATRIX& GetMatProjection() { return matProjection; }

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// ������x�N�g���̎擾
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3& GetUp() { return up; }
#pragma endregion

#pragma region �����o�ϐ�
private:
	XMMATRIX matView; // �r���[�s��
	XMMATRIX matProjection; // �ˉe�s��
	XMFLOAT3 eye; // ���_���W
	XMFLOAT3 target; // �����_���W
	XMFLOAT3 up; // ������x�N�g��
#pragma endregion
};