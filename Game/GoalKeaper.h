#pragma once

/// <summary>
/// �S�[���L�[�p�[�N���X
/// </summary>
class GoalKeaper
{
private: // �萔
	const float BodyHeight = 3.5f; // �s�b�`�̍������O�Ƃ������̃I�u�W�F�N�g�̍���
	const float footHeight = 1.5f; // �@�@�@�@�@�@�V�@�@�@�@�@�@���̍���

	//const XMFLOAT2 bodySize{ 3.0f / 2, 6.5f }; // �̂̑傫���ix ���a�A y �����j
	//const XMFLOAT3 footSize{ 5.5f / 2, 1.5f / 2, 1.5f / 2 }; // ���̑傫���ix �� / 2�A y ���� / 2�A z �[�� / 2�j


public: // �����o�֐�
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Draw();

private: // �����o�ϐ�
	// �`��p
	//std::unique_ptr<Obj3dModel> mode; // 3d���f��
	//std::unique_ptr<Obj3dObject> object; // 3d�I�u�W�F�N�g

	//XMFLOAT3 position{}; // ���W
	//XMFLOAT3 angle{}; // �p�x

	//// �����p
	//btCollisionShape* cShape = nullptr; // �Փˌ`��
	//btRigidBody* rBody = nullptr; // ����

	//btVector3 velocity{}; // ���x

	//btVector3 startPos{ -19, BodyHeight, 0 }; // �������W
};
