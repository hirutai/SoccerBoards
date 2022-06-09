#include "Goal.h"

#include "Obj3dModel.h"
#include "Obj3dObject.h"

using namespace std;

void Goal::Initialzie(const std::string& modelName, const UINT& netTexNum)
{
#pragma region 3Dモデル生成
	ModelInit(modelName, netTexNum);
#pragma endregion

#pragma region 3Dオブジェクト生成
	ObjectInit();
#pragma endregion
}

void Goal::Update()
{
#pragma region 更新
	objectGoal->Update();
#pragma endregion
}

void Goal::Draw(ID3D12GraphicsCommandList* cmdList)
{
#pragma region 描画
	objectGoal->Draw(cmdList);
#pragma endregion
}

void Goal::ModelInit(const std::string& modelName, const UINT& netTexNum)
{
	modelGoal = make_unique<Obj3dModel>();
	modelGoal->Initialize(modelName, netTexNum);
}

void Goal::ObjectInit()
{
	objectGoal = make_unique<Obj3dObject>();
	objectGoal->Initialize(modelGoal.get());
}