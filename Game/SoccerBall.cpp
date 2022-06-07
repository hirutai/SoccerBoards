#include "SoccerBall.h"

#include "Obj3dModel.h"
#include "Obj3dObject.h"

using namespace std;

void SoccerBall::Initialize(const std::string& modelName, const UINT& ballTexNum)
{
	// モデルの初期化
	ModelInit(modelName, ballTexNum);
	// オブジェクトの初期化
	ObjectInit();
}

void SoccerBall::Update()
{
	// 数値の設定
	object->SetPosition(position);
	object->SetRotation(angle);

	object->Update(true);
}

void SoccerBall::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
}

void SoccerBall::ModelInit(const std::string& modelName, const UINT& netTexNum)
{
	model = make_unique<Obj3dModel>();
	model->Initialize(modelName, netTexNum);
}

void SoccerBall::ObjectInit()
{
	object = make_unique<Obj3dObject>();
	object->Initialize(model.get());
	object->Update();
}