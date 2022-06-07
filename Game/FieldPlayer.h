#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Obj3dModel.h"
#include "Obj3dObject.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

/// <summary>
/// フィールドプレイヤークラス
/// </summary>
class FieldPlayer
{
#pragma region エイリアス
private:
	// std::を省略
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	//using XMFLOAT4 = DirectX::XMFLOAT4;
#pragma endregion

#pragma region 列挙型
	// 選手の状態
	enum class PlayerStatus
	{
		Normal, // 通常時
		LeftRotating, // 左回転中
		RightRotating, // 右回転中
	};
#pragma endregion

#pragma region 定数
private:
	const float BodyHeight = 3.5f; // ピッチの高さを０とした時のオブジェクトの高さ
	const float footHeight = 1.5f; // 　　　　　　〃　　　　　　足の高さ

	const XMFLOAT2 bodySize{ 3.0f / 2, 6.5f }; // 体の大きさ（x 半径、 y 高さ）
	const XMFLOAT3 footSize{ 5.5f / 2, 1.5f / 2, 1.5f / 2 }; // 足の大きさ（x 幅 / 2、 y 高さ / 2、 z 深さ / 2）
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();
#pragma endregion

#pragma region メンバ変数
private:
	std::unique_ptr<Obj3dModel> model; // 3dモデル
	std::unique_ptr<Obj3dObject> object; // 3dオブジェクト

	XMFLOAT3 position{}; // 座標
	XMFLOAT3 angle{}; // 角度

	PlayerStatus status = PlayerStatus::Normal; // 状態

	// 物理オブジェクト関連
	btVector3 velocity{}; // 速度
	btVector3 rotation{}; // 回転量

	float rotationValue = 0; // 回転値

	// 体用
	btCollisionShape* cShapeoftheBody; // 衝突形状
	btRigidBody* rBodyoftheBody; // 剛体

	// 足用
	btCollisionShape* cShapeoftheFoot; // 衝突形状
	btRigidBody* rBodyoftheFoot; // 剛体

	btVector3 startPos{}; // 初期座標
#pragma endregion
};
