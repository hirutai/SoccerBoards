#pragma once

#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
#include <memory>

class Obj3dModel;
class Obj3dObject;

/// <summary>
/// �T�b�J�[�{�[��
/// </summary>
class SoccerBall
{
#pragma region �G�C���A�X
private:
	// std::���ȗ�
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region �����o�֐�(public)
public:
	// ������
	void Initialize(const std::string& modelName, const UINT& ballTexNum);
	// �X�V
	void Update();
	// �`��
	void Draw();

	// ���W�̐ݒ�
	void SetPosition(const XMFLOAT3& physicsPosition) { position = physicsPosition; }
	// �p�x�̐ݒ�
	void SetAngle(const XMFLOAT3& physicsAngle) { angle = physicsAngle; }

	// ���W�̎擾
	const XMFLOAT3& GetPosition() { return position; }
	// �p�x�̎擾
	const XMFLOAT3& GetAngle() { return angle; }
#pragma endregion

#pragma region �����o�֐�(private)
private:
	// 3d���f���̏�����
	void ModelInit(const std::string& modelName, const UINT& ballTexNum);

	// 3d�I�u�W�F�N�g�̏�����
	void ObjectInit();
#pragma endregion

#pragma region �����o�ϐ�
private:
	unique_ptr<Obj3dModel> model; // ���f��
	unique_ptr<Obj3dObject> object; // �I�u�W�F�N�g

	XMFLOAT3 position{}; // ���W
	XMFLOAT3 angle{}; // �p�x
#pragma endregion
};