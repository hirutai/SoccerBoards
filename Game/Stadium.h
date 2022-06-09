#pragma once
#include <d3d12.h>
#include <string>
#include <DirectXMath.h>
#include <memory>

class Obj3dModel;
class Obj3dObject;

/// <summary>
/// �X�^�W�A���N���X
/// </summary>
class Stadium
{
#pragma region �G�C���A�X
private:
	// std::���ȗ�
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region �����o�֐�
public:
	// ������
	void Initialize(const UINT& pitchTexNum, const UINT& wallTexNum);
	// �X�V
	void Update();
	// �`��
	void Draw();
#pragma endregion

#pragma region �����o�֐�
private:
	// 3d���f���̏�����
	void ModelInit(std::unique_ptr<Obj3dModel>& model, const std::string& modelName, const UINT& texNum);

	// 3d�I�u�W�F�N�g�̏�����
	void ObjectInit(std::unique_ptr<Obj3dObject>& object, Obj3dModel* model);
#pragma endregion

#pragma region �����o�ϐ�
private:
	std::unique_ptr<Obj3dModel> modelPitch; // 3d���f��
	std::unique_ptr<Obj3dObject> objectPitch; // 3d�I�u�W�F�N�g

	std::unique_ptr<Obj3dModel> modelWall; // 3d���f��
	std::unique_ptr<Obj3dObject> objectWall; // 3d�I�u�W�F�N�g
#pragma endregion
};