#pragma once

#include <memory>
#include <DirectXMath.h>

#include "BaseScene.h"
#include "WindowsApi.h"
#include "Sprite.h"
#include "Obj3dModel.h"
#include "Obj3dObject.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

// 前方宣言
class Stadium; // スタジアムクラス

#pragma region テクスチャ番号
enum texnumber
{
	DebugFont,
	// オブジェクトの色用
	SoccerBall, // ボールの色
	MyPlayer, // 味方選手の色
	CpuPlayer, // 相手選手の色
	Pitch, // ピッチの色
	Wall, // 壁の色
	Net, // ゴールの色
	// 前景、背景用
	BackGround, // 背景
	TITLE, // タイトル
	Ex, // 操作説明
	Rule, // 勝利条件
	KickOff, // キックオフ
	Score, // スコア
	Goal, // ゴール！
	Result, // 試合結果
};
#pragma endregion

#pragma region シーン番号
enum class Scene
{
	Title, // タイトル
	Explanation, // 操作説明
	CameraRotate, // カメラの回転
	KickOffDirecting, // キックオフ時の演出
	Game, // ゲーム
	MyGoal, // 味方ゴール時
	MyGoalDirecting, // 味方ゴール時の演出
	CpuGoal, // 相手ゴール時
	CpuGoalDirecting, // 相手ゴール時の演出
	ResetStatus, // ゲーム中のリセット
	Result, // 結果
};
#pragma endregion

#pragma region 選手の状態
enum class PlayerStatus
{
	Normal, // 通常時
	LeftRotating, // 左回転中
	RightRotating, // 右回転中
};
#pragma endregion

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlay : public BaseScene
{
#pragma region エイリアス
private:
	// std::を省略
	template <class T> using unique_ptr = std::unique_ptr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
#pragma endregion

#pragma region メンバ関数
public:
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 背景描画
	void DrawBackSprite();
	// 3Dオブジェクト描画
	void DrawObject();
	// 前景描画
	void DrawFrontSprite();
	// 解放
	void Finalize();
#pragma endregion

#pragma region カメラ
#define VERTICALEYE -30.0f // 縦向き視点

	XMFLOAT3 cameraEye{ 0.0f, 30.0f, VERTICALEYE - cameraEye.x }; // カメラの視点
	XMFLOAT3 cameraTarget{ cameraEye.x / 4, 0.0f, 0.0f }; // カメラ注視点

	float cameraRotation = 0.18f; // 回転量
#pragma endregion

#pragma region オブジェクト全体設定
private: // メンバ
	const float objectSize = 1.0f; // オブジェクトサイズの倍率
	const float BodyHeight = 3.5f; // ピッチの高さを０とした時のオブジェクトの高さ
	const float footHeight = 1.5f; // 　　　　　　〃　　　　　　足の高さ
#pragma endregion

#pragma region スプライト アンカーポイント設定用
	XMFLOAT2 vertCenter{ 0.5f,0.5f }; // 中央頂点
	XMFLOAT2 vertLeftCenter{ 0.0f,0.5f }; // 左中央頂点
	XMFLOAT2 vertRightCenter{ 1.0f,0.5f }; // 右中央頂点
#pragma endregion

#pragma region スプライト 初期座標指定用
	XMFLOAT2 screenUpCenter{ WindowsApi::windowWidth / 2, 50.0f }; // 画面上中央
	XMFLOAT2 screenCenter{ WindowsApi::windowWidth / 2, WindowsApi::windowHeight / 2 }; // 画面中央
#pragma endregion

#pragma region サッカーボール
private: // メンバ
	// 描画用
	unique_ptr<Obj3dModel> mBall; // モデル
	unique_ptr<Obj3dObject> oBall; // オブジェクト

	XMFLOAT3 ballPos{}; // 座標
	XMFLOAT3 ballAng{}; // 角度

	// 物理用
	btVector3 ballStartPos{ 0, 20, 0 }; // ボールの初期座標
	const float ballRadius = 3.0f / 2; // ボールの半径
	btCollisionShape* cShapeBall = nullptr; // 衝突形状
	btRigidBody* rBodyBall = nullptr; // 剛体
#pragma endregion

#pragma region 選手全体設定
private: // 定数
#define PLAYERNUM 5 // 各チームフィールドプレイヤー数
#define KEAPERNUM 1 // 各チームゴールキーパー数

#define PI 3.14159265 // 円周率
#define CIRCLELENGTH PI * 2.0f // 一周の長さ 

#define L1STARTPOS 11.0f // １ライン目の選手の初期Z座標
#define PLAYERINTERVAL 5.5f // 選手のZ座標の間隔

private: // メンバ
	const XMFLOAT2 bodySize{ 3.0f / 2, 6.5f }; // 体の大きさ（x 半径、 y 高さ）
	const XMFLOAT3 footSize{ 5.5f / 2, 1.5f / 2, 1.5f / 2 }; // 足の大きさ（x 幅 / 2、 y 高さ / 2、 z 深さ / 2）
#pragma endregion

#pragma region 味方選手設定
#define mL1MINPOS -16.0f // １ライン目の選手の移動範囲
#define mL1MAXPOS 16.0f
#define mL2MINPOS 1.0f // ２ライン目の選手の移動範囲
#define mL2MAXPOS 16.0f
#define mL3MINPOS -16.0f // ３ライン目の選手の移動範囲
#define mL3MAXPOS -1.0f
#define mL4MINPOS 1.0f // ４ライン目の選手の移動範囲
#define mL4MAXPOS 16.0f
#define mL5MINPOS -16.0f // ５ライン目の選手の移動範囲
#define mL5MAXPOS 16.0f
#define mKMINPOS -5.0f // キーパーの移動範囲
#define mKMAXPOS 5.0f

#define myPlayerSpeVal 0.2f // フィールドプレイヤの速さ
#define myPlayerRotVal 0.05f // 味方選手の回転量

#define myKeaperSpeVal 0.1f // キーパーの速さ

#define TWOSECONDS 120.0f // 2秒 

private: // フィールドプレイヤー
	// 描画用
	unique_ptr<Obj3dModel> modelMyBody; // モデル
	unique_ptr<Obj3dObject> objectMyBody[PLAYERNUM]; // 3Dオブジェクト

	XMFLOAT3 myPlayerPos[PLAYERNUM]{}; // 座標
	XMFLOAT3 myPlayerAng[PLAYERNUM]{}; // 回転角

	PlayerStatus myPlayerStatus = PlayerStatus::Normal; // 選手の状態

	// 全体物理用
	btVector3 myPlayerVel[PLAYERNUM]{}; // 速度
	btVector3 myPlayerRot[PLAYERNUM]{}; // 回転量

	float myPlayerRotationValue = 0; // 回転用変数

	// 体用
	// 物理用
	btCollisionShape* cShapeMyBody[PLAYERNUM]; // 衝突形状
	btRigidBody* rBodyMyBody[PLAYERNUM]; // 剛体

	btVector3 myBodyStartPos[PLAYERNUM]{}; // 体の初期座標

	// 足用
	// 物理用
	btCollisionShape* cShapeMyFoot[PLAYERNUM]; // 衝突形状
	btRigidBody* rBodyMyFoot[PLAYERNUM]; // 剛体

	btVector3 myFootStartPos[PLAYERNUM]{}; // 足の初期座標
private: // ゴールキーパー
	// 描画用
	unique_ptr<Obj3dModel> modelMyKeaper; // モデル
	unique_ptr<Obj3dObject> objectMyKeaper; // 3Dオブジェクト

	XMFLOAT3 myKeaperPos{}; // 座標
	XMFLOAT3 myKeaperAng{}; // 回転角

	// 物理用
	btCollisionShape* cShapeMyKeaper = nullptr; // 衝突形状
	btRigidBody* rBodyMyKeaper = nullptr; // 剛体

	btVector3 myKeaperVel{}; // 速度

	btVector3 myKeaperStartPos{ -19, BodyHeight, 0 }; // ゴールキーパーの初期座標
#pragma endregion

#pragma region 相手選手
private: // 定数
#define cL1MINPOS -16.0f // １ライン目の選手の移動範囲
#define cL1MAXPOS 16.0f
#define cL2MINPOS -16.0f // ２ライン目の選手の移動範囲
#define cL2MAXPOS -1.0f
#define cL3MINPOS 1.0f // ３ライン目の選手の移動範囲
#define cL3MAXPOS 16.0f
#define cL4MINPOS -16.0f // ４ライン目の選手の移動範囲
#define cL4MAXPOS -1.0f
#define cL5MINPOS -16.0f // ５ライン目の選手の移動範囲
#define cL5MAXPOS 16.0f
#define cKMINPOS -5.0f // キーパーの移動範囲
#define cKMAXPOS 5.0f

private: // フィールドプレイヤー
	// 全体描画用
	unique_ptr<Obj3dModel> modelCpuBody; // モデル
	unique_ptr<Obj3dObject> objectCpuBody[PLAYERNUM]; // 3Dオブジェクト

	XMFLOAT3 cpuPlayerPos[PLAYERNUM]{}; // 座標
	XMFLOAT3 cpuPlayerAng[PLAYERNUM]{}; // 角度

	// 全体物理用
	btVector3 cpuPlayerVel[PLAYERNUM]{}; // 速度
	btVector3 cpuPlayerRot[PLAYERNUM]{}; // 回転量

	// 体
	// 物理用
	btCollisionShape* cShapeCpuBody[PLAYERNUM]; // 衝突形状
	btRigidBody* rBodyCpuBody[PLAYERNUM]; // 剛体

	btVector3 cpuBodyStartPos[PLAYERNUM]{}; // 相手の体の初期座標

	// 足用
	// 物理用
	btCollisionShape* cShapeCpuFoot[PLAYERNUM]; // 衝突形状
	btRigidBody* rBodyCpuFoot[PLAYERNUM]; // 剛体

	btVector3 cpuFootStartPos[PLAYERNUM]{}; // 足の初期座標

private: // ゴールキーパー
	unique_ptr<Obj3dModel> modelCpuKeaper; // モデル
	unique_ptr<Obj3dObject> objectCpuKeaper; // 3Dオブジェクト

	XMFLOAT3 cpuKeaperPos{}; // 座標
	XMFLOAT3 cpuKeaperAng{}; // 角度

	// 物理用
	btCollisionShape* cShapeCpuKeaper = nullptr; // 衝突形状
	btRigidBody* rBodyCpuKeaper = nullptr; // 剛体

	btVector3 cpuKeaperVel{}; // 移動量

	btVector3 cpuKeaperStartPos{ 19, BodyHeight, 0 }; // 相手ゴールキーパーの初期座標
#pragma endregion

	// AI用
	float pSwitchingVelTime = 0; // フィールドプレイヤの移動量切り替え用
	float pVelSwitching = 0.1f; // 移動量切り替え用
	float pSwitchingRotTime = 0; // フィールドプレイヤの回転量切り替え用
	bool rotatePlayerFlag = false; // 回転中か
	float cpuPlayerRotationValue = 0.0f; // 回転用変数
	float laRSwitching = 0.1f; // 左右切り替え用

	float kSwitchingTime = 0; // キーパーの移動量切り替え用
	float kVelSwitching = 0.1f; // 移動量切り替え用

#pragma region スタジアム
	Stadium* stadium = nullptr; // スタジアム

private: // メンバ
	const XMFLOAT3 pitchSize{ 40.0f / 2, 0.0f, 30.0f / 2 }; // ピッチのx 幅 / 2、 y 高さ / 2、 z 深さ / 2

	// ピッチの物理用
	btCollisionShape* cShapeGround = nullptr; // 衝突形状
	btRigidBody* rBodyGround = nullptr; // 剛体

#define lrWALLNUM 2 // 左右壁の数

	float lrWallHalf = 8.0f / 2; // 左右壁のZ軸の半分のサイズ

	const XMFLOAT3 lrWallSize{ 0.1f / 2, 100.0f / 2, lrWallHalf }; // 左右壁のx 幅 / 2、 y 高さ / 2、 z 深さ / 2

	// 左壁の物理用
	btCollisionShape* cShapeLWall[2]; // 衝突形状
	btRigidBody* rBodyLWall[2]; // 剛体

	// 右壁の物理用
	btCollisionShape* cShapeRWall[2]; // 衝突形状
	btRigidBody* rBodyRWall[2]; // 剛体

	const XMFLOAT3 fbWallSize{ 40.0f / 2, 100.0f / 2, 0.1f / 2 }; // 前奥壁のx 幅 / 2, y 高さ / 2、 z 深さ / 2

	// 前奥壁の物理用
	btCollisionShape* cShapeFBWall[2]; // 衝突形状
	btRigidBody* rBodyFBWall[2]; // 剛体
#pragma endregion

#pragma region ゴール
	// 味方ゴールの描画用
	unique_ptr<Obj3dModel> mMyGoal; // モデル
	unique_ptr<Obj3dObject> oMyGoal; // オブジェクト

	// 味方ゴールの物理用
	btCollisionShape* cShapeMyGoal = nullptr; // 衝突形状
	btRigidBody* rBodyMyGoal = nullptr; // 剛体

	XMFLOAT3 myGoalPos{ -20.0f, 0.0f, 0.0f }; // 座標

	// 相手ゴールの描画用
	unique_ptr<Obj3dModel> mCpuGoal; // モデル
	unique_ptr<Obj3dObject> oCpuGoal; // オブジェクト

	// 相手ゴールの物理用
	btCollisionShape* cShapeCpuGoal = nullptr; // 衝突形状
	btRigidBody* rBodyCpuGoal = nullptr; // 剛体

	XMFLOAT3 cpuGoalPos{ 20.0f, 0.0f, 0.0f }; // 座標
#pragma endregion

#pragma region 背景画像
	unique_ptr<Sprite> backGround; // 背景
#pragma endregion

#pragma region 操作説明
	unique_ptr<Sprite> ex; // 操作説明
#pragma endregion

#pragma region キックオフ画像
#define KICKOFFVEL 7.5f // 文字の移動量
#define LEFTEND -WindowsApi::windowWidth / 2 // 左端

	unique_ptr<Sprite> kickoff; // キックオフの文字

	XMFLOAT2 kickoffStartPos{ WindowsApi::windowWidth, WindowsApi::windowHeight / 2 }; // キックオフの初期座標

	XMFLOAT2 kickoffPos{ kickoffStartPos }; // キックオフの座標

	int stopTime = 0; // 中央に止まっている時間をカウント用
#pragma endregion

#pragma region スコア画像
#define VICTORYSCORE 3 // 勝利スコア

	unique_ptr<Sprite> myScoreNumber; // 味方得点用数字
	unique_ptr<Sprite> cpuScoreNumber; // 相手得点用数字

	unsigned int myScore = 0; // 味方の得点
	XMFLOAT2 myScoreOrigin = { 0.5f, 0.0f }; // 味方得点の原点
	XMFLOAT2 myScoreSize = { 100.0f, 100.0f }; // 味方得点のサイズ

	XMFLOAT2 myScorePos{ WindowsApi::windowWidth / 2 - 200, 50 }; // 味方得点の座標

	unsigned int cpuScore = 0; // 相手の得点
	XMFLOAT2 cpuScoreOrigin = { 0.5f, 0.0f }; // 相手の得点画像の原点
	XMFLOAT2 cpuScoreSize = { 100.0f, 100.0f }; // 相手の得点画像のサイズ

	XMFLOAT2 cpuScorePos{ WindowsApi::windowWidth / 2 + 200, 50 }; // 相手得点の座標
#pragma endregion

#pragma region ゴール画像
#define GOALCHARWIDTH 500.0f // ゴール！の文字の元横幅
#define GOALCHARHEIGHT 100.0f // 　　　　　　  の元縦幅

#define ORIGINALSIZE 2.0f // 元のサイズの倍率

#define EXPRATE 0.01f // 拡大率

	unique_ptr<Sprite> goalCha; // ゴール！の文字

	float goalChaMag = 0.1f; // ゴール！の文字の倍率

	XMFLOAT2 goalStartSize{ GOALCHARWIDTH, GOALCHARHEIGHT }; // ゴール！の初期サイズ

	XMFLOAT2 goalChaSize{ goalStartSize }; // ゴール！のサイズ

#pragma endregion 試合結果
	unique_ptr<Sprite> result; // 試合結果
#pragma region 

#pragma region 物理
private: // メンバ
	// 物理世界の初期化
	void PhysicsInit();

	// 物理オブジェクトの作成
	// 地面の作成
	void CreateGround();
	// 壁の作成
	void CreateWall();
	// ボールの作成
	void CreateBall();
	// 味方フィールドプレイヤーの作成（体と足）
	void CreateMyPlayer();
	// 味方キーパーの作成
	void CreateMyKeaper();
	// 相手フィールドプレイヤーの作成
	void CreateCpuPlayer();
	// 相手キーパーの作成
	void CreateCpuKeaper();

	// 物理世界の更新
	void PhysicsUpdate();

	// 物理オブジェクト移動時の更新
	void PhysicsMoveUpdate();

	// 物理オブジェクトと描画オブジェクトの座標のリンク
	void SetPosition(XMFLOAT3& objPos, const btVector3& phyPos);
	// 回転角のリンク
	void SetRotation(XMFLOAT3& objRot, const btVector3& phyRot);

	// 物理世界用変数
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr; // 衝突構成
	btCollisionDispatcher* dispatcher = nullptr; // 衝突ディスパッチャー
	btBroadphaseInterface* overlappingPairCache = nullptr; // ブロードフェーズ
	btSequentialImpulseConstraintSolver* solver = nullptr; // 制約ソルバ
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr; // 物理世界
	btAlignedObjectArray<btCollisionShape*> collisionShapes; // 衝突形状配列
#pragma endregion

#pragma region ゲーム
private:
	// スプライト生成関数
	void CreateSprite();

	// オブジェクトの更新処理
	void ObjectUpdate();

	// ゲームのリセット
	void ResetGame();

	// ゲーム内のリセット
	void ResetStatus();

	// 味方のゴール
	void MyGoal();

	// 相手のゴール
	void CpuGoal();

	Scene scene = Scene::Title; // シーン管理
#pragma endregion
};