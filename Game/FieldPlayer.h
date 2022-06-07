#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Obj3dModel.h"
#include "Obj3dObject.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

/// <summary>
/// �t�B�[���h�v���C���[�N���X
/// </summary>
class FieldPlayer
{
#pragma region �G�C���A�X
private:
	// std::���ȗ�
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	//using XMFLOAT4 = DirectX::XMFLOAT4;
#pragma endregion

#pragma region �񋓌^
	// �I��̏��
	enum class PlayerStatus
	{
		Normal, // �ʏ펞
		LeftRotating, // ����]��
		RightRotating, // �E��]��
	};
#pragma endregion

#pragma region �萔
private:
	const float BodyHeight = 3.5f; // �s�b�`�̍������O�Ƃ������̃I�u�W�F�N�g�̍���
	const float footHeight = 1.5f; // �@�@�@�@�@�@�V�@�@�@�@�@�@���̍���

	const XMFLOAT2 bodySize{ 3.0f / 2, 6.5f }; // �̂̑傫���ix ���a�A y �����j
	const XMFLOAT3 footSize{ 5.5f / 2, 1.5f / 2, 1.5f / 2 }; // ���̑傫���ix �� / 2�A y ���� / 2�A z �[�� / 2�j
#pragma endregion

#pragma region �����o�֐�
public:
	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Draw();
#pragma endregion

#pragma region �����o�ϐ�
private:
	std::unique_ptr<Obj3dModel> model; // 3d���f��
	std::unique_ptr<Obj3dObject> object; // 3d�I�u�W�F�N�g

	XMFLOAT3 position{}; // ���W
	XMFLOAT3 angle{}; // �p�x

	PlayerStatus status = PlayerStatus::Normal; // ���

	// �����I�u�W�F�N�g�֘A
	btVector3 velocity{}; // ���x
	btVector3 rotation{}; // ��]��

	float rotationValue = 0; // ��]�l

	// �̗p
	btCollisionShape* cShapeoftheBody; // �Փˌ`��
	btRigidBody* rBodyoftheBody; // ����

	// ���p
	btCollisionShape* cShapeoftheFoot; // �Փˌ`��
	btRigidBody* rBodyoftheFoot; // ����

	btVector3 startPos{}; // �������W
#pragma endregion
};
