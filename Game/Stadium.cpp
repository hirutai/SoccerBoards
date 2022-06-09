#include "Stadium.h"

#include "Obj3dModel.h"
#include "Obj3dObject.h"

using namespace std;

void Stadium::Initialize(const UINT& pitchTexNum, const UINT& wallTexNum)
{
#pragma region 3D���f������
	// �s�b�`
	ModelInit(modelPitch, "pitch", pitchTexNum);
	// ��
	ModelInit(modelWall, "wall", wallTexNum);
#pragma endregion

#pragma region 3D�I�u�W�F�N�g����
	// �s�b�`
	ObjectInit(objectPitch, modelPitch.get());
	// ��
	ObjectInit(objectWall, modelWall.get());
#pragma endregion
}

void Stadium::Update()
{
#pragma region �X�V
	// �s�b�`
	objectPitch->Update();
	// ��
	objectWall->Update();
#pragma endregion
}

void Stadium::Draw()
{
#pragma region �`��
	// �s�b�`
	objectPitch->Draw();
	// ��
	objectWall->Draw();
#pragma endregion
}

void Stadium::ModelInit(std::unique_ptr<Obj3dModel>& model, const std::string& modelName, const UINT& texNum)
{
	model = make_unique<Obj3dModel>();
	model->Initialize(modelName, texNum);
}

void Stadium::ObjectInit(std::unique_ptr<Obj3dObject>& object, Obj3dModel* model)
{
	object = make_unique<Obj3dObject>();
	object->Initialize(model);
}