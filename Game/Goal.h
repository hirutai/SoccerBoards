#pragma once
#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
#include <memory>

class Obj3dModel;
class Obj3dObject;
class Sprite;

/// <summary>
/// �S�[��
/// </summary>
class Goal
{
#pragma region �G�C���A�X
private:
	// std::���ȗ�
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region �萔

#pragma endregion

#pragma region �����o�֐�
public:
	// ������
	void Initialzie(const std::string& modelName, const UINT& netTexNum);
	// �X�V
	void Update();
	// �`��
	void Draw(ID3D12GraphicsCommandList* cmdList);
#pragma endregion

#pragma region �����o�֐�
private:
	// 3d���f���̏�����
	void ModelInit(const std::string& modelName, const UINT& netTexNum);

	// 3d�I�u�W�F�N�g�̏�����
	void ObjectInit();
#pragma endregion

#pragma region �����o�ϐ�
private:
	// �I�u�W�F�N�g�֘A
	std::unique_ptr<Obj3dModel> modelGoal; // ���f��
	std::unique_ptr<Obj3dObject> objectGoal; // �I�u�W�F�N�g
#pragma endregion
};