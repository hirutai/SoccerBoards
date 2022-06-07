#pragma once

/// <summary>
/// ゴールキーパークラス
/// </summary>
class GoalKeaper
{
private: // 定数
	const float BodyHeight = 3.5f; // ピッチの高さを０とした時のオブジェクトの高さ
	const float footHeight = 1.5f; // 　　　　　　〃　　　　　　足の高さ

	//const XMFLOAT2 bodySize{ 3.0f / 2, 6.5f }; // 体の大きさ（x 半径、 y 高さ）
	//const XMFLOAT3 footSize{ 5.5f / 2, 1.5f / 2, 1.5f / 2 }; // 足の大きさ（x 幅 / 2、 y 高さ / 2、 z 深さ / 2）


public: // メンバ関数
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();

private: // メンバ変数
	// 描画用
	//std::unique_ptr<Obj3dModel> mode; // 3dモデル
	//std::unique_ptr<Obj3dObject> object; // 3dオブジェクト

	//XMFLOAT3 position{}; // 座標
	//XMFLOAT3 angle{}; // 角度

	//// 物理用
	//btCollisionShape* cShape = nullptr; // 衝突形状
	//btRigidBody* rBody = nullptr; // 剛体

	//btVector3 velocity{}; // 速度

	//btVector3 startPos{ -19, BodyHeight, 0 }; // 初期座標
};
