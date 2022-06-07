#include "Stadium.h"

#include "Obj3dModel.h"
#include "Obj3dObject.h"

using namespace std;

void Stadium::Initialize(const UINT& pitchTexNum, const UINT& wallTexNum)
{
#pragma region 3Dモデル生成
	// ピッチ
	ModelInit(modelPitch, "pitch", pitchTexNum);
	// 壁
	ModelInit(modelWall, "wall", wallTexNum);
#pragma endregion

#pragma region 3Dオブジェクト生成
	// ピッチ
	ObjectInit(objectPitch, modelPitch.get());
	// 壁
	ObjectInit(objectWall, modelWall.get());
#pragma endregion
}

void Stadium::Update()
{
#pragma region 更新
	// ピッチ
	objectPitch->Update();
	// 壁
	objectWall->Update();
#pragma endregion
}

void Stadium::Draw(ID3D12GraphicsCommandList* cmdList)
{
#pragma region 描画
	// ピッチ
	objectPitch->Draw(cmdList);
	// 壁
	objectWall->Draw(cmdList);
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
	object->Update();
}