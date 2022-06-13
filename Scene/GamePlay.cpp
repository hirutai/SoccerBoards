#include "GamePlay.h"
#include "SceneManager.h"
#include "Input.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Stadium.h"
#include "Goal.h"
#include "SoccerBall.h"
#include "Result.h"
#include "Title.h"

using namespace std;
using namespace DirectX;

void GamePlay::Initialize()
{
#pragma region テクスチャ読み込み
	TextureManager* texManager = TextureManager::GetInstance();

	texManager->LoadTexture(DebugFont, L"Resources/debugfont.png");
	texManager->LoadTexture(SOCCERBALL, L"Resources/Sprite/soccerball.png");
	texManager->LoadTexture(MyPlayer, L"Resources/Sprite/red.png");
	texManager->LoadTexture(CpuPlayer, L"Resources/Sprite/blue.png");
	texManager->LoadTexture(Pitch, L"Resources/Sprite/pitch.png");
	texManager->LoadTexture(Wall, L"Resources/Sprite/green.png");
	texManager->LoadTexture(Net, L"Resources/Sprite/net.png");
	texManager->LoadTexture(BackGround, L"Resources/Sprite/background.png");
	texManager->LoadTexture(KickOff, L"Resources/Sprite/kickoff.png");
	texManager->LoadTexture(Score, L"Resources/Sprite/score.png");
	texManager->LoadTexture(GOAL, L"Resources/Sprite/goal!.png");
#pragma endregion

#pragma region スプライト生成
	// キックオフ画像
	kickoff = make_unique<Sprite>();
	kickoff->Initialize(KickOff);
	kickoff->SetAnchorPoint(vertCenter); // 頂点を中心に設定
	kickoff->SetPosition(kickoffPos.x, kickoffPos.y); // 画面右に配置

	// スコア用画像
	// 味方スコア
	myScoreNumber = make_unique<Sprite>();
	myScoreNumber->Initialize(Score);
	myScoreNumber->SetAnchorPoint(vertLeftCenter); // 頂点を左辺中央に設定
	myScoreNumber->SetPosition(myScorePos.x, myScorePos.y);
	myScoreNumber->SetRect(myScoreOrigin.x, myScoreOrigin.y, myScoreSize.x, myScoreSize.y);

	// 相手スコア
	cpuScoreNumber = make_unique<Sprite>();
	cpuScoreNumber->Initialize(Score);
	cpuScoreNumber->SetAnchorPoint(vertRightCenter); // 頂点を左辺中央に設定
	cpuScoreNumber->SetPosition(cpuScorePos.x, cpuScorePos.y);
	cpuScoreNumber->SetRect(cpuScoreOrigin.x, cpuScoreOrigin.y, cpuScoreSize.x, cpuScoreSize.y);

	// ゴール画像
	goalCha = make_unique<Sprite>();
	goalCha->Initialize(GOAL);
	goalCha->SetAnchorPoint(vertCenter); // 頂点を中心に設定
	goalCha->SetPosition(screenCenter.x, screenCenter.y); // 画面中央に配置
	goalCha->SetSize(goalChaSize.x, goalChaSize.y);

	// 背景画面
	backGround = make_unique<Sprite>();
	backGround->Initialize(BackGround);
#pragma endregion

#pragma region 3Dモデル生成
	// 味方フィールドプレイヤー
	modelMyBody = make_unique<Obj3dModel>();
	modelMyBody->Initialize("player", MyPlayer);

	// 味方ゴールキーパー
	modelMyKeaper = make_unique<Obj3dModel>();
	modelMyKeaper->Initialize("player", MyPlayer);

	// 相手フィールドプレイヤー
	modelCpuBody = make_unique<Obj3dModel>();
	modelCpuBody->Initialize("player", CpuPlayer);

	// 相手ゴールキーパー
	modelCpuKeaper = make_unique<Obj3dModel>();
	modelCpuKeaper->Initialize("player", CpuPlayer);
#pragma endregion

#pragma region 3Dオブジェクト生成
	for (int i = 0; i < PLAYERNUM; i++)
	{
		// 味方フィールドプレイヤー（体）
		objectMyBody[i] = make_unique<Obj3dObject>();
		objectMyBody[i]->Initialize(modelMyBody.get());

		// 相手フィールドプレイヤー（体）
		objectCpuBody[i] = make_unique<Obj3dObject>();
		objectCpuBody[i]->Initialize(modelCpuBody.get());
	}

	// 味方ゴールキーパー
	objectMyKeaper = make_unique<Obj3dObject>();
	objectMyKeaper->Initialize(modelMyKeaper.get());

	// 相手ゴールキーパー
	objectCpuKeaper = make_unique<Obj3dObject>();
	objectCpuKeaper->Initialize(modelCpuKeaper.get());
#pragma endregion

	// スタジアム
	stadium = new Stadium();
	stadium->Initialize(Pitch, Wall); // 初期化

	// 味方ゴール
	myGoal = new Goal();
	myGoal->Initialzie("mygoal", Net);
	myGoal->SetPosition(XMFLOAT3(-4, 0, 0));

	// 相手ゴール
	cpuGoal = new Goal();
	cpuGoal->Initialzie("cpugoal", Net);
	cpuGoal->SetPosition(XMFLOAT3(4, 0, 0));

	// サッカーボール
	soccerBall = new SoccerBall();
	soccerBall->Initialize("ball", SOCCERBALL);

#pragma region その他初期化
	// 物理世界の初期化
	PhysicsInit();

	// 物理オブジェクト作成
	// 地面
	CreateGround();
	// 壁
	CreateWall();
	// ゴール壁
	CreateGoalWall();
	// 天井
	CreateCeiling();
	// ボール
	CreateBall();
	// 味方フィールドプレイヤー（体と足）
	CreateMyPlayer();
	// 味方ゴールキーパー
	CreateMyKeaper();
	// 相手フィールドプレイヤー
	CreateCpuPlayer();
	// 相手ゴールキーパー
	CreateCpuKeaper();

	ResetGame();
#pragma endregion
}

void GamePlay::Update()
{
	Input* input = Input::GetInstance();

	switch (scene)
	{
	case Scene::CameraRotate: // カメラ回転
		if (cameraEye.x > VERTICALEYE)
		{
			cameraEye.x -= cameraRotation; // 縦向きになるまで回転
		}
		else
		{
			cameraEye.x = VERTICALEYE; // 縦向きに固定
			scene = Scene::KickOffDirecting; // キックオフ演出へ
		}

		Camera::GetInstance()->SetEye(XMFLOAT3(cameraEye.x, 30.0f, VERTICALEYE - cameraEye.x));

		Camera::GetInstance()->SetTarget(XMFLOAT3(cameraEye.x / 4, 0.0f, 0.0f));

		PhysicsUpdate();

		ObjectUpdate();
		break;
	case Scene::KickOffDirecting: // キックオフ演出タイミング

		if (kickoffPos.x > WindowsApi::windowWidth / 2) // キックオフの文字が画面中央に止まるまで
		{
			kickoffPos.x -= KICKOFFVEL;
		}
		else
		{
			stopTime++; // 中央に止まっている時間をカウント

			if (stopTime > TWOSECONDS) // ２秒経過したら
			{
				if (kickoffPos.x > LEFTEND) // キックオフの文字が左端に到達するまで
				{
					kickoffPos.x -= KICKOFFVEL;
				}
				else
				{
					stopTime = 0;
					scene = Scene::Game; // ゲームへ
				}
			}
		}

		kickoff->SetPosition(kickoffPos.x, kickoffPos.y);

		break;
	case Scene::Game: // ゲーム

		for (int i = 0; i < PLAYERNUM; i++)
		{
			cpuPlayerVel[i].setValue(pVelSwitching, 0.0f, 0.0f);

			if (pSwitchingVelTime >= TWOSECONDS * 5.0f) // 一定時間経った時
			{
				pSwitchingVelTime = 0.0f; // 時間のリセット
				pVelSwitching *= -1; // 移動量切り替え
			}
			else
			{
				pSwitchingVelTime++;
			}
		}

		cpuKeaperVel.setValue(0.0f, 0.0f, kVelSwitching);

		if (kSwitchingTime >= TWOSECONDS * 5.0f)
		{
			kSwitchingTime = 0.0f;
			kVelSwitching *= -1;
		}
		else
		{
			kSwitchingTime++;
		}

		// 物理世界の更新
		PhysicsUpdate();

		ObjectUpdate();

		XMFLOAT3 ballPos = soccerBall->GetPosition();

		if (ballPos.x > cpuGoalPos.x) // 相手ゴールにボールが入った時
		{
			scene = Scene::MyGoal; // 味方ゴール時シーンへ
		}
		if (ballPos.x < myGoalPos.x) // 味方ゴールにボールが入った時
		{
			scene = Scene::CpuGoal; // 相手ゴール時シーンへ
		}

		break;
	case Scene::MyGoal: // 味方ゴール時
		// 味方のゴール
		MyGoal();

		ObjectUpdate();

		scene = Scene::MyGoalDirecting; // ゴール演出へ

		break;
	case Scene::MyGoalDirecting: // 味方ゴール演出タイミング
		if (goalChaMag >= ORIGINALSIZE) // 一定のサイズに拡大されるまで
		{
			//goalChaMag = 0.1f;

			stopTime++; // 中央に止まっている時間をカウント

			if (stopTime > TWOSECONDS) // ２秒経過したら
			{
				stopTime = 0;

				scene = Scene::ResetStatus; // リセット
			}
		}
		else
		{
			goalChaMag += EXPRATE; // ゴール！の文字を拡大
		}

		goalChaSize = XMFLOAT2(GOALCHARWIDTH * goalChaMag, GOALCHARHEIGHT * goalChaMag);
		goalCha->SetSize(goalChaSize.x, goalChaSize.y);

		break;
	case Scene::CpuGoal: // 相手ゴール時
		// 相手のゴール
		CpuGoal();

		ObjectUpdate();

		scene = Scene::CpuGoalDirecting; // ゴール演出へ

		break;
	case Scene::CpuGoalDirecting: // 相手ゴール演出タイミング
		if (goalChaMag >= ORIGINALSIZE) // 一定のサイズに拡大されるまで
		{
			//goalChaMag = 0.1f;

			stopTime++; // 中央に止まっている時間をカウント

			if (stopTime > TWOSECONDS) // ２秒経過したら
			{
				stopTime = 0;

				scene = Scene::ResetStatus; // リセット
			}
		}
		else
		{
			goalChaMag += EXPRATE; // ゴール！の文字を拡大
		}

		goalChaSize = XMFLOAT2(GOALCHARWIDTH * goalChaMag, GOALCHARHEIGHT * goalChaMag);
		goalCha->SetSize(goalChaSize.x, goalChaSize.y);

		break;
	case Scene::ResetStatus: // ゲーム中のリセット
		ResetStatus();

		if (myScore == 3 || cpuScore == 3)
		{
			// タイトルへ
			BaseScene* scene = new Title();
			sceneManager->SetNextScene(scene);
		}

		scene = Scene::KickOffDirecting; // リスタート

		break;
	}
}

void GamePlay::DrawBackSprite()
{
#pragma region 背景描画
	Sprite::SetPipeline();

	backGround->Draw(); // 描画
#pragma endregion
}

void GamePlay::DrawObject()
{
#pragma region 3Dオブジェクト描画
	// スタジアム
	stadium->Draw();

	// 味方ゴール
	myGoal->Draw();
	// 相手ゴール
	cpuGoal->Draw();

	// 味方フィールドプレイヤー
	for (int i = 0; i < PLAYERNUM; i++)
	{
		// 体
		objectMyBody[i]->Draw();
	}

	// 相手フィールドプレイヤー
	for (int i = 0; i < PLAYERNUM; i++)
	{
		// 体
		objectCpuBody[i]->Draw();
	}

	// 味方ゴールキーパー
	objectMyKeaper->Draw();
	// 相手ゴールキーパー
	objectCpuKeaper->Draw();

	// ボール
	soccerBall->Draw();
#pragma endregion
}

void GamePlay::DrawFrontSprite()
{
#pragma region スプライト描画
	// スプライト描画前処理
	Sprite::SetPipeline();

	switch (scene)
	{
	case Scene::KickOffDirecting:
		myScoreNumber->Draw();
		cpuScoreNumber->Draw();
		kickoff->Draw();

		break;
	case Scene::Game:
		myScoreNumber->Draw();
		cpuScoreNumber->Draw();

		break;
	case Scene::MyGoalDirecting:
		myScoreNumber->Draw();
		cpuScoreNumber->Draw();
		goalCha->Draw();

		break;
	case Scene::CpuGoalDirecting:
		myScoreNumber->Draw();
		cpuScoreNumber->Draw();
		goalCha->Draw();

		break;
	}
#pragma endregion
}

void GamePlay::Finalize()
{
#pragma region 物理関連の解放
	// 剛体の削除
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}

		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	// 衝突形状の削除
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	collisionShapes.clear();

	// 物理世界の削除
	delete dynamicsWorld;

	// ソルバの削除
	delete solver;

	// ブロードフェーズの削除
	delete overlappingPairCache;

	// 衝突ディスパッチャーの削除
	delete dispatcher;

	// 衝突構成の削除
	delete collisionConfiguration;
#pragma endregion

	// スタジアムの削除
	delete stadium;
}

void GamePlay::PhysicsInit()
{
	// 衝突構成
	collisionConfiguration = new btDefaultCollisionConfiguration();

	// 衝突ディスパッチャー
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// ブロードフェーズ
	overlappingPairCache = new btDbvtBroadphase();

	// 制約ソルバ
	solver = new btSequentialImpulseConstraintSolver();

	// 物理世界作成
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}

void GamePlay::CreateGround()
{
	// 衝突形状の設定
	cShapeGround = new btBoxShape(btVector3(btScalar(pitchSize.x), btScalar(pitchSize.y), btScalar(pitchSize.z)));
	// 衝突形状配列に追加
	collisionShapes.push_back(cShapeGround);

	btTransform startTransform; // 初期座標用
	startTransform.setIdentity();

	btScalar mass(0.); // 質量

	btVector3 localInertia(0, 0, 0); // 慣性

	// 初期座標を設定
	startTransform.setOrigin(btVector3(0, 0, 0));

	// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeGround->calculateLocalInertia(mass, localInertia);
	}

	// 移動状態
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// 剛体作成に必要な情報
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeGround, localInertia);

	// 剛体作成
	rBodyGround = new btRigidBody(rbInfo);
	rBodyGround->setFriction(0.1f);

	// 物理世界に剛体を登録
	dynamicsWorld->addRigidBody(rBodyGround);
}

void GamePlay::CreateWall()
{
	for (int i = 0; i < lrWALLNUM; i++)
	{
		btScalar posZ = 12.75 - 12.75 * (i + i); // 左右壁のZ座標 i=0→12.75(ゴール奥の壁) i=1→-12.75(ゴール手前の壁)

		btScalar mass(0.); // 質量
		bool isDynamic = (mass != 0.f);

		// 左の壁
		{
			float lWallPos = -24; // 各左壁の座標

			// 衝突形状の設定
			cShapeLWall[i] = new btBoxShape(btVector3(btScalar(lrWallSize.x), btScalar(lrWallSize.y), btScalar(lrWallSize.z)));
			// 衝突形状配列に追加
			collisionShapes.push_back(cShapeLWall[i]);

			btTransform startTransform; // 初期座標用
			startTransform.setIdentity();

			btVector3 localInertia(0, 0, 0); // 慣性

			// 初期座標を設定
			startTransform.setOrigin(btVector3(lWallPos, lrWallSize.y, posZ));

			// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
			if (isDynamic)
			{
				cShapeLWall[i]->calculateLocalInertia(mass, localInertia);
			}

			// 移動状態
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// 剛体作成に必要な情報
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeLWall[i], localInertia);

			// 剛体作成
			rBodyLWall[i] = new btRigidBody(rbInfo);

			// 物理世界に剛体を登録
			dynamicsWorld->addRigidBody(rBodyLWall[i]);
		}

		// 右の壁
		{
			float rWallPos = 24; // 各右壁の座標

			// 衝突形状の設定
			cShapeRWall[i] = new btBoxShape(btVector3(btScalar(lrWallSize.x), btScalar(lrWallSize.y), btScalar(lrWallSize.z)));
			// 衝突形状配列に追加
			collisionShapes.push_back(cShapeRWall[i]);

			btTransform startTransform; // 初期座標用
			startTransform.setIdentity();

			btVector3 localInertia(0, 0, 0); // 慣性

			// 初期座標を設定
			startTransform.setOrigin(btVector3(rWallPos, lrWallSize.y, posZ));

			// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
			if (isDynamic)
			{
				cShapeRWall[i]->calculateLocalInertia(mass, localInertia);
			}

			// 移動状態
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// 剛体作成に必要な情報
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeRWall[i], localInertia);

			// 剛体作成
			rBodyRWall[i] = new btRigidBody(rbInfo);

			// 物理世界に剛体を登録
			dynamicsWorld->addRigidBody(rBodyRWall[i]);
		}

		// 上下の壁
		{
			float bWallStartPos = 18; // 奥壁の座標

			// 衝突形状の設定
			cShapeFBWall[i] = new btBoxShape(btVector3(btScalar(fbWallSize.x), btScalar(fbWallSize.y), btScalar(fbWallSize.z)));
			// 衝突形状配列に追加
			collisionShapes.push_back(cShapeFBWall[i]);

			btTransform startTransform; // 初期座標用
			startTransform.setIdentity();

			btVector3 localInertia(0, 0, 0); // 慣性

			// i = 0 前壁のz座標 i = 1 奥壁のz座標
			btScalar posZ = bWallStartPos - bWallStartPos * (i + i);

			// 初期座標を設定
			startTransform.setOrigin(btVector3(0, fbWallSize.y, posZ));

			// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
			if (isDynamic)
			{
				cShapeFBWall[i]->calculateLocalInertia(mass, localInertia);
			}

			// 移動状態
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// 剛体作成に必要な情報
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeFBWall[i], localInertia);

			// 剛体作成
			rBodyFBWall[i] = new btRigidBody(rbInfo);

			// 物理世界に剛体を登録
			dynamicsWorld->addRigidBody(rBodyFBWall[i]);
		}
	}
}

void GamePlay::CreateGoalWall()
{
	for (int i = 0; i < 2; i++)
	{
		// 衝突形状の設定
		cShapeGoalWall[i] = new btBoxShape(btVector3(btScalar(1.0f), btScalar(6.0f), btScalar(7.25f)));
		// 衝突形状配列に追加
		collisionShapes.push_back(cShapeGoalWall[i]);

		btTransform startTransform; // 初期座標用
		startTransform.setIdentity();

		btScalar mass(0.); // 質量

		btVector3 localInertia(0, 0, 0); // 慣性

		btScalar posX = -24.0f + 48.0f * i;

		// 初期座標を設定
		startTransform.setOrigin(btVector3(posX, 12.0f, 0.0f));

		bool isDynamic = (mass != 0.f);

		// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
		if (isDynamic)
		{
			cShapeGoalWall[i]->calculateLocalInertia(mass, localInertia);
		}

		// 移動状態
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		// 剛体作成に必要な情報
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeGoalWall[i], localInertia);

		// 剛体作成
		rBodyGoalWall[i] = new btRigidBody(rbInfo);

		// 物理世界に剛体を登録
		dynamicsWorld->addRigidBody(rBodyGoalWall[i]);
	}
}

void GamePlay::CreateCeiling()
{
	// 衝突形状の設定
	cShapeCeiling = new btBoxShape(btVector3(btScalar(pitchSize.x), btScalar(pitchSize.y), btScalar(pitchSize.z)));
	// 衝突形状配列に追加
	collisionShapes.push_back(cShapeCeiling);

	btTransform startTransform; // 初期座標用
	startTransform.setIdentity();

	btScalar mass(0.); // 質量

	btVector3 localInertia(0, 0, 0); // 慣性

	// 初期座標を設定
	startTransform.setOrigin(btVector3(0, 12.0f, 0));

	// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeCeiling->calculateLocalInertia(mass, localInertia);
	}

	// 移動状態
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// 剛体作成に必要な情報
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeCeiling, localInertia);

	// 剛体作成
	rBodyCeiling = new btRigidBody(rbInfo);

	// 物理世界に剛体を登録
	dynamicsWorld->addRigidBody(rBodyCeiling);
}

void GamePlay::CreateBall()
{
	// 衝突形状の設定
	cShapeBall = new btSphereShape(btScalar(ballRadius));

	// 衝突形状配列に追加
	collisionShapes.push_back(cShapeBall);

	btTransform startTransform;	// 初期座標用
	startTransform.setIdentity();

	btScalar mass(1.f); // 質量

	btVector3 localInertia(0, 0, 0); // 慣性

	// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeBall->calculateLocalInertia(mass, localInertia);
	}

	// 初期座標を設定
	startTransform.setOrigin(ballStartPos);

	// 移動状態
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// 剛体作成に必要な情報
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeBall, localInertia);

	// 剛体作成
	rBodyBall = new btRigidBody(rbInfo);

	// 摩擦の設定
	rBodyBall->setRollingFriction(0.5f);
	rBodyBall->setSpinningFriction(0.5f);
	rBodyBall->setFriction(0.5f);
	rBodyBall->setAnisotropicFriction(cShapeBall->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

	// 物理世界に剛体を登録
	dynamicsWorld->addRigidBody(rBodyBall);
}

void GamePlay::CreateMyPlayer()
{
	for (int i = 0; i < PLAYERNUM; i++)
	{
		btScalar posZ = L1STARTPOS - PLAYERINTERVAL * i;

		// 各選手の体の初期座標を設定
		myBodyStartPos[i] = btVector3(mL1MINPOS, BodyHeight, posZ); // １列目
		// 各選手の足の初期座標を設定
		myFootStartPos[i] = btVector3(mL1MINPOS, footHeight, posZ); // １列目
	}

	for (int i = 0; i < PLAYERNUM; i++)
	{
		// 体
		{
			// 衝突形状の設定
			cShapeMyBody[i] = new btCapsuleShape(btScalar(bodySize.x), btScalar(bodySize.y));
			// 衝突形状配列に追加
			collisionShapes.push_back(cShapeMyBody[i]);

			btTransform startTransform; // 初期座標用
			startTransform.setIdentity();

			btScalar mass(0.f); // 質量

			btVector3 localInertia(0, 0, 0); // 慣性

			// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
			{
				cShapeMyBody[i]->calculateLocalInertia(mass, localInertia);
			}

			// 初期座標を設定
			startTransform.setOrigin(myBodyStartPos[i]);

			// 移動状態
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// 剛体作成に必要な情報
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeMyBody[i], localInertia);

			// 剛体作成
			rBodyMyBody[i] = new btRigidBody(rbInfo);

			rBodyMyBody[i]->setCollisionFlags(rBodyMyBody[i]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rBodyMyBody[i]->setActivationState(DISABLE_DEACTIVATION);

			// 物理世界に剛体を登録
			dynamicsWorld->addRigidBody(rBodyMyBody[i]);
		}

		// 足
		{
			// 衝突形状の設定
			cShapeMyFoot[i] = new btBoxShape(btVector3(btScalar(footSize.x), btScalar(footSize.y), btScalar(footSize.z)));
			// 衝突形状配列に追加
			collisionShapes.push_back(cShapeMyFoot[i]);

			btTransform startTransform; // 初期座標用
			startTransform.setIdentity();

			btScalar mass(0.f); // 質量

			btVector3 localInertia(0, 0, 0); // 慣性

			// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
			{
				cShapeMyFoot[i]->calculateLocalInertia(mass, localInertia);
			}

			// 初期座標を設定
			startTransform.setOrigin(myFootStartPos[i]);

			// 移動状態
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// 剛体作成に必要な情報
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeMyFoot[i], localInertia);

			// 剛体作成
			rBodyMyFoot[i] = new btRigidBody(rbInfo);

			rBodyMyFoot[i]->setCollisionFlags(rBodyMyFoot[i]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rBodyMyFoot[i]->setActivationState(DISABLE_DEACTIVATION);

			// 物理世界に剛体を登録
			dynamicsWorld->addRigidBody(rBodyMyFoot[i]);
		}
	}
}

void GamePlay::CreateMyKeaper()
{
	// 衝突形状の設定
	cShapeMyKeaper = new btCapsuleShape(btScalar(bodySize.x), btScalar(bodySize.y));
	// 衝突形状配列に追加
	collisionShapes.push_back(cShapeMyKeaper);

	btTransform startTransform; // 初期座標用
	startTransform.setIdentity();

	btScalar mass(0.f); // 質量

	btVector3 localInertia(0, 0, 0); // 慣性

	// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeMyKeaper->calculateLocalInertia(mass, localInertia);
	}

	// 初期座標を設定
	startTransform.setOrigin(myKeaperStartPos);

	// 移動状態
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// 剛体作成に必要な情報
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeMyKeaper, localInertia);

	// 剛体作成
	rBodyMyKeaper = new btRigidBody(rbInfo);

	rBodyMyKeaper->setCollisionFlags(rBodyMyKeaper->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	rBodyMyKeaper->setActivationState(DISABLE_DEACTIVATION);

	// 物理世界に剛体を登録
	dynamicsWorld->addRigidBody(rBodyMyKeaper);
}

void GamePlay::CreateCpuPlayer()
{
	for (int i = 0; i < PLAYERNUM; i++)
	{
		btScalar posZ = L1STARTPOS - PLAYERINTERVAL * i;

		// 各選手の初期座標を設定
		cpuBodyStartPos[i] = btVector3(cL1MINPOS, BodyHeight, posZ); // １列目
		// 各選手の足の初期座標を設定
		cpuFootStartPos[i] = btVector3(cL1MINPOS, footHeight, posZ); // １列目
	}

	for (int i = 0; i < PLAYERNUM; i++)
	{
		// 体
		{
			// 衝突形状の設定
			cShapeCpuBody[i] = new btCapsuleShape(btScalar(bodySize.x), btScalar(bodySize.y));
			// 衝突形状配列に追加
			collisionShapes.push_back(cShapeCpuBody[i]);

			btTransform startTransform; // 初期座標用
			startTransform.setIdentity();

			btScalar mass(0.f); // 質量

			btVector3 localInertia(0, 0, 0); // 慣性

			// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
			{
				cShapeCpuBody[i]->calculateLocalInertia(mass, localInertia);
			}

			// 初期座標を設定
			startTransform.setOrigin(cpuBodyStartPos[i]);

			// 移動状態
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// 剛体作成に必要な情報
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeCpuBody[i], localInertia);

			// 剛体作成
			rBodyCpuBody[i] = new btRigidBody(rbInfo);

			rBodyCpuBody[i]->setCollisionFlags(rBodyCpuBody[i]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rBodyCpuBody[i]->setActivationState(DISABLE_DEACTIVATION);

			// 物理世界に剛体を登録
			dynamicsWorld->addRigidBody(rBodyCpuBody[i]);
		}

		// 足
		{
			// 衝突形状の設定
			cShapeCpuFoot[i] = new btBoxShape(btVector3(btScalar(footSize.x), btScalar(footSize.y), btScalar(footSize.z)));
			// 衝突形状配列に追加
			collisionShapes.push_back(cShapeCpuFoot[i]);

			btTransform startTransform; // 初期座標用
			startTransform.setIdentity();

			btScalar mass(0.f); // 質量

			btVector3 localInertia(0, 0, 0); // 慣性

			// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
			bool isDynamic = (mass != 0.f);
			if (isDynamic)
			{
				cShapeCpuFoot[i]->calculateLocalInertia(mass, localInertia);
			}

			// 初期座標を設定
			startTransform.setOrigin(cpuFootStartPos[i]);

			// 移動状態
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			// 剛体作成に必要な情報
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeCpuFoot[i], localInertia);

			// 剛体作成
			rBodyCpuFoot[i] = new btRigidBody(rbInfo);

			rBodyCpuFoot[i]->setCollisionFlags(rBodyCpuFoot[i]->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			rBodyCpuFoot[i]->setActivationState(DISABLE_DEACTIVATION);

			// 物理世界に剛体を登録
			dynamicsWorld->addRigidBody(rBodyCpuFoot[i]);
		}
	}
}

void GamePlay::CreateCpuKeaper()
{
	// 衝突形状の設定
	cShapeCpuKeaper = new btCapsuleShape(btScalar(bodySize.x), btScalar(bodySize.y));
	// 衝突形状配列に追加
	collisionShapes.push_back(cShapeCpuKeaper);

	btTransform startTransform; // 初期座標用
	startTransform.setIdentity();

	btScalar mass(0.f); // 質量

	btVector3 localInertia(0, 0, 0); // 慣性

	// 質量が０以外なら動的（TRUE）、それ以外は静的（FALSE）
	bool isDynamic = (mass != 0.f);
	if (isDynamic)
	{
		cShapeCpuKeaper->calculateLocalInertia(mass, localInertia);
	}

	// 初期座標を設定
	startTransform.setOrigin(cpuKeaperStartPos);

	// 移動状態
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	// 剛体作成に必要な情報
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cShapeCpuKeaper, localInertia);

	// 剛体作成
	rBodyCpuKeaper = new btRigidBody(rbInfo);

	rBodyCpuKeaper->setCollisionFlags(rBodyCpuKeaper->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	rBodyCpuKeaper->setActivationState(DISABLE_DEACTIVATION);

	// 物理世界に剛体を登録
	dynamicsWorld->addRigidBody(rBodyCpuKeaper);
}

void GamePlay::PhysicsUpdate()
{
	Input* input = Input::GetInstance();

	// 物理世界の更新
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);

#pragma region ボールの更新
	{
		btTransform trans; // ボールの状態格納用

		if (rBodyBall && rBodyBall->getMotionState()) // ボールの状態を取得
		{
			rBodyBall->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = rBodyBall->getWorldTransform();
		}

		// 描画オブジェクト関連
		// ボールの座標の取得
		btVector3 transPos = trans.getOrigin();
		// XMFLOAT3型に変換
		XMFLOAT3 physicsPos = XMFLOAT3(transPos.getX(), transPos.getY(), transPos.getZ());
		// 座標の設定
		soccerBall->SetPosition(physicsPos);

		// 回転量の取得
		btQuaternion transRot = trans.getRotation();
		// オイラー角の取得
		btScalar rollX, pitchY, yawZ;
		transRot.getEulerZYX(yawZ, pitchY, rollX);
		// 度に変換
		btVector3 convertRot = btVector3(XMConvertToDegrees((float)rollX), XMConvertToDegrees((float)pitchY), XMConvertToDegrees((float)yawZ));
		// XMFLOAT3型に変換
		XMFLOAT3 physicsRot = XMFLOAT3(convertRot.getX(), convertRot.getY(), convertRot.getZ());
		// 角度の設定
		soccerBall->SetAngle(physicsRot);
	}
#pragma endregion

#pragma region 味方フィールドプレイヤーの更新
	{
		btTransform trans;  // 体の状態格納用
		btTransform trans2; // 足の状態格納用

		for (int i = 0; i < PLAYERNUM; i++)
		{
			// 物理オブジェクトの状態を取得
			if (rBodyMyBody[i] && rBodyMyBody[i]->getMotionState()) // 選手の状態を取得
			{
				rBodyMyBody[i]->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = rBodyMyBody[i]->getWorldTransform();
			}

			if (rBodyMyFoot[i] && rBodyMyFoot[i]->getMotionState()) // 選手の状態を取得
			{
				rBodyMyFoot[i]->getMotionState()->getWorldTransform(trans2);
			}
			else
			{
				trans2 = rBodyMyFoot[i]->getWorldTransform();
			}

			// 移動
			myPlayerVel[i].setValue(0, 0, 0);

			if (input->PushKey(DIK_W)) // 奥へ移動
			{
				myPlayerVel[i].setX(myPlayerSpeVal);
			}
			else if (input->PushKey(DIK_S)) // 前へ移動
			{
				myPlayerVel[i].setX(-myPlayerSpeVal);
			}

			// 体、足の座標の更新
			trans.setOrigin(trans.getOrigin() + myPlayerVel[i]);
			trans2.setOrigin(trans2.getOrigin() + myPlayerVel[i]);

			bool cond1 = myPlayerStatus == PlayerStatus::Normal; // 通常時
			bool cond2 = myPlayerStatus == PlayerStatus::LeftRotating; // 左回転時
			bool cond3 = myPlayerStatus == PlayerStatus::RightRotating; // 右回転時

			if (cond1) // 左回転してない時
			{
				if (input->TriggerKey(DIK_Q))
				{
					myPlayerStatus = PlayerStatus::LeftRotating; // 左回転開始
				}
			}
			else if (cond2)
			{
				if (myPlayerRotationValue < -CIRCLELENGTH)
				{
					myPlayerRotationValue = 0;
					myPlayerStatus = PlayerStatus::Normal;
				}
				else
				{
					myPlayerRotationValue -= myPlayerRotVal;
				}
			}

			if (cond1) // 右回転してない時
			{
				if (input->TriggerKey(DIK_E)) // 右回転
				{
					myPlayerStatus = PlayerStatus::RightRotating; // 右回転開始
				}
			}
			else if (cond3)
			{
				if (myPlayerRotationValue > CIRCLELENGTH)
				{
					myPlayerRotationValue = 0;
					myPlayerStatus = PlayerStatus::Normal;
				}
				else
				{
					myPlayerRotationValue += myPlayerRotVal;
				}
			}

			myPlayerRot[i].setX(myPlayerRotationValue);

			// 体、足の角度の更新
			btQuaternion transRot = trans.getRotation();

			btQuaternion q = btQuaternion(myPlayerRot[i].getX(), myPlayerRot[i].getY(), myPlayerRot[i].getZ());
			trans.setRotation(q);

			btQuaternion trans2Rot = trans2.getRotation();

			trans2.setRotation(q);

			// 移動制限
			const btScalar& bx = trans.getOrigin().getX();
			const btScalar& by = trans.getOrigin().getY();
			const btScalar& bz = trans.getOrigin().getZ();

			const btScalar& fx = trans2.getOrigin().getX();
			const btScalar& fy = trans2.getOrigin().getY();
			const btScalar& fz = trans2.getOrigin().getZ();
			switch (i)
			{
			case 0: // １列目
				// 体について
				if (bx < mL1MINPOS)
				{
					trans.setOrigin(btVector3(mL1MINPOS, by, bz));
				}
				else if (bx > mL1MAXPOS)
				{
					trans.setOrigin(btVector3(mL1MAXPOS, by, bz));
				}

				// 足について
				if (fx < mL1MINPOS)
				{
					trans2.setOrigin(btVector3(mL1MINPOS, fy, fz));
				}
				else if (fx > mL1MAXPOS)
				{
					trans2.setOrigin(btVector3(mL1MAXPOS, fy, fz));
				}

				break;
			case 1: // ２列目
				if (bx < mL2MINPOS)
				{
					trans.setOrigin(btVector3(mL2MINPOS, by, bz));
				}
				else if (bx > mL2MAXPOS)
				{
					trans.setOrigin(btVector3(mL2MAXPOS, by, bz));
				}

				// 足について
				if (fx < mL2MINPOS)
				{
					trans2.setOrigin(btVector3(mL2MINPOS, fy, fz));
				}
				else if (fx > mL2MAXPOS)
				{
					trans2.setOrigin(btVector3(mL2MAXPOS, fy, fz));
				}

				break;
			case 2: // ３列目
				if (bx < mL3MINPOS)
				{
					trans.setOrigin(btVector3(mL3MINPOS, by, bz));
				}
				else if (bx > mL3MAXPOS)
				{
					trans.setOrigin(btVector3(mL3MAXPOS, by, bz));
				}

				// 足について
				if (fx < mL3MINPOS)
				{
					trans2.setOrigin(btVector3(mL3MINPOS, fy, fz));
				}
				else if (fx > mL3MAXPOS)
				{
					trans2.setOrigin(btVector3(mL3MAXPOS, fy, fz));
				}

				break;
			case 3: // ４列目
				if (bx < mL4MINPOS)
				{
					trans.setOrigin(btVector3(mL4MINPOS, by, bz));
				}
				else if (bx > mL4MAXPOS)
				{
					trans.setOrigin(btVector3(mL4MAXPOS, by, bz));
				}

				// 足について
				if (fx < mL1MINPOS)
				{
					trans2.setOrigin(btVector3(mL4MINPOS, fy, fz));
				}
				else if (fx > mL4MAXPOS)
				{
					trans2.setOrigin(btVector3(mL4MAXPOS, fy, fz));
				}

				break;
			default: // ５列目
				if (bx < mL5MINPOS)
				{
					trans.setOrigin(btVector3(mL5MINPOS, by, bz));
				}
				else if (bx > mL5MAXPOS)
				{
					trans.setOrigin(btVector3(mL5MAXPOS, by, bz));
				}

				// 足について
				if (fx < mL5MINPOS)
				{
					trans2.setOrigin(btVector3(mL5MINPOS, fy, fz));
				}
				else if (fx > mL5MAXPOS)
				{
					trans2.setOrigin(btVector3(mL5MAXPOS, fy, fz));
				}

				break;
			}

			rBodyMyBody[i]->getMotionState()->setWorldTransform(trans);
			rBodyMyFoot[i]->getMotionState()->setWorldTransform(trans2);

			// 体関連
			{
				// 描画オブジェクト関連
				// 座標のセット
				btVector3 physicsPos = trans.getOrigin();
				SetPosition(myPlayerPos[i], physicsPos);

				// 角度のセット
				btQuaternion physicsRot = trans.getRotation();

				btScalar rollX, pitchY, yawZ;

				physicsRot.getEulerZYX(yawZ, pitchY, rollX);

				btVector3 rot = btVector3(XMConvertToDegrees((float)rollX), XMConvertToDegrees((float)pitchY), XMConvertToDegrees((float)yawZ));

				SetRotation(myPlayerAng[i], rot);
			}
		}
	}
#pragma endregion

#pragma region 味方キーパーの更新
	{
		btTransform trans;  // キーパーの状態格納用

		// 物理オブジェクト関連
		if (rBodyMyKeaper && rBodyMyKeaper->getMotionState()) // 選手の状態を取得
		{
			rBodyMyKeaper->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = rBodyMyKeaper->getWorldTransform();
		}

		// 移動
		myKeaperVel.setValue(0, 0, 0);

		if (input->PushKey(DIK_A))
		{
			myKeaperVel.setZ(myKeaperSpeVal);
		}
		else if (input->PushKey(DIK_D))
		{
			myKeaperVel.setZ(-myKeaperSpeVal);
		}

		// 座標の更新
		trans.setOrigin(trans.getOrigin() + myKeaperVel);

		// 移動制限
		if (trans.getOrigin().getZ() < mKMINPOS)
		{
			trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), mKMINPOS));
		}
		else if (trans.getOrigin().getZ() > mKMAXPOS)
		{
			trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), mKMAXPOS));
		}

		rBodyMyKeaper->getMotionState()->setWorldTransform(trans);

		// 描画オブジェクト関連
		// 座標のセット
		btVector3 physicsPos = trans.getOrigin();
		SetPosition(myKeaperPos, physicsPos);
	}
#pragma endregion

#pragma region 相手フィールドプレイヤーの更新
	{
		btTransform trans;  // 体の状態格納用
		btTransform trans2; // 足の状態格納用

		for (int i = 0; i < PLAYERNUM; i++)
		{
			if (rBodyCpuBody[i] && rBodyCpuBody[i]->getMotionState()) // 選手の状態を取得
			{
				rBodyCpuBody[i]->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = rBodyCpuBody[i]->getWorldTransform();
			}

			if (rBodyCpuFoot[i] && rBodyCpuFoot[i]->getMotionState()) // 選手の状態を取得
			{
				rBodyCpuFoot[i]->getMotionState()->getWorldTransform(trans2);
			}
			else
			{
				trans2 = rBodyCpuFoot[i]->getWorldTransform();
			}

			//cpuPlayerVel[i].setValue(pVelSwitching, 0.0f, 0.0f);

			//if (pSwitchingVelTime >= TWOSECONDS * 5.0f) // 一定時間経った時
			//{
			//	pSwitchingVelTime = 0.0f; // 時間のリセット
			//	pVelSwitching *= -1; // 移動量切り替え
			//}
			//else
			//{
			//	pSwitchingVelTime++;
			//}

			// 体、足の座標の更新
			trans.setOrigin(trans.getOrigin() + cpuPlayerVel[i]);
			trans2.setOrigin(trans2.getOrigin() + cpuPlayerVel[i]);

			// 体、足の角度の更新
			btQuaternion transRot = trans.getRotation();

			btQuaternion q = btQuaternion(cpuPlayerRot[i].getX(), cpuPlayerRot[i].getY(), cpuPlayerRot[i].getZ());
			trans.setRotation(q);

			btQuaternion trans2Rot = trans2.getRotation();

			trans2.setRotation(q);

			// 移動制限
			const btScalar& bx = trans.getOrigin().getX();
			const btScalar& by = trans.getOrigin().getY();
			const btScalar& bz = trans.getOrigin().getZ();

			const btScalar& fx = trans2.getOrigin().getX();
			const btScalar& fy = trans2.getOrigin().getY();
			const btScalar& fz = trans2.getOrigin().getZ();
			switch (i)
			{
			case 0: // １列目
				// 体について
				if (bx < cL1MINPOS)
				{
					trans.setOrigin(btVector3(cL1MINPOS, by, bz));
				}
				else if (bx > cL1MAXPOS)
				{
					trans.setOrigin(btVector3(cL1MAXPOS, by, bz));
				}

				// 足について
				if (fx < cL1MINPOS)
				{
					trans2.setOrigin(btVector3(cL1MINPOS, fy, fz));
				}
				else if (fx > cL1MAXPOS)
				{
					trans2.setOrigin(btVector3(cL1MAXPOS, fy, fz));
				}

				break;
			case 1: // ２列目
				if (bx < cL2MINPOS)
				{
					trans.setOrigin(btVector3(cL2MINPOS, by, bz));
				}
				else if (bx > cL2MAXPOS)
				{
					trans.setOrigin(btVector3(cL2MAXPOS, by, bz));
				}

				// 足について
				if (fx < cL2MINPOS)
				{
					trans2.setOrigin(btVector3(cL2MINPOS, fy, fz));
				}
				else if (fx > cL2MAXPOS)
				{
					trans2.setOrigin(btVector3(cL2MAXPOS, fy, fz));
				}

				break;
			case 2: // ３列目
				if (bx < cL3MINPOS)
				{
					trans.setOrigin(btVector3(cL3MINPOS, by, bz));
				}
				else if (bx > cL3MAXPOS)
				{
					trans.setOrigin(btVector3(cL3MAXPOS, by, bz));
				}

				// 足について
				if (fx < cL3MINPOS)
				{
					trans2.setOrigin(btVector3(cL3MINPOS, fy, fz));
				}
				else if (fx > cL3MAXPOS)
				{
					trans2.setOrigin(btVector3(cL3MAXPOS, fy, fz));
				}

				break;
			case 3: // ４列目
				if (bx < cL4MINPOS)
				{
					trans.setOrigin(btVector3(cL4MINPOS, by, bz));
				}
				else if (bx > cL4MAXPOS)
				{
					trans.setOrigin(btVector3(cL4MAXPOS, by, bz));
				}

				// 足について
				if (fx < cL1MINPOS)
				{
					trans2.setOrigin(btVector3(cL4MINPOS, fy, fz));
				}
				else if (fx > cL4MAXPOS)
				{
					trans2.setOrigin(btVector3(cL4MAXPOS, fy, fz));
				}

				break;
			default: // ５列目
				if (bx < cL5MINPOS)
				{
					trans.setOrigin(btVector3(cL5MINPOS, by, bz));
				}
				else if (bx > cL5MAXPOS)
				{
					trans.setOrigin(btVector3(cL5MAXPOS, by, bz));
				}

				// 足について
				if (fx < cL5MINPOS)
				{
					trans2.setOrigin(btVector3(cL5MINPOS, fy, fz));
				}
				else if (fx > cL5MAXPOS)
				{
					trans2.setOrigin(btVector3(cL5MAXPOS, fy, fz));
				}

				break;
			}

			rBodyCpuBody[i]->getMotionState()->setWorldTransform(trans);
			rBodyCpuFoot[i]->getMotionState()->setWorldTransform(trans2);

			// 描画オブジェクト関連
			// 座標のセット
			btVector3 physicsPos = trans.getOrigin();
			SetPosition(cpuPlayerPos[i], physicsPos);

			// 角度のセット
			btQuaternion physicsRot = trans.getRotation();

			btScalar rollx, pitchy, yawz;

			physicsRot.getEulerZYX(yawz, pitchy, rollx);

			btVector3 rot = btVector3(XMConvertToDegrees((float)rollx), XMConvertToDegrees((float)pitchy), XMConvertToDegrees((float)yawz));

			SetRotation(cpuPlayerAng[i], rot);
		}
	}
#pragma endregion

#pragma region 相手キーパーの更新
	{
		btTransform trans;  // キーパーの状態格納用

		if (rBodyCpuKeaper && rBodyCpuKeaper->getMotionState()) // 選手の状態を取得
		{
			rBodyCpuKeaper->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = rBodyCpuKeaper->getWorldTransform();
		}

		/*cpuKeaperVel.setValue(0.0f, 0.0f, kVelSwitching);

		if (kSwitchingTime >= TWOSECONDS * 5.0f)
		{
			kSwitchingTime = 0.0f;
			kVelSwitching *= -1;
		}
		else
		{
			kSwitchingTime++;
		}*/

		if (trans.getOrigin().getZ() < cKMINPOS)
		{
			trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), cKMINPOS));
		}
		else if (trans.getOrigin().getZ() > cKMAXPOS)
		{
			trans.setOrigin(btVector3(trans.getOrigin().getX(), trans.getOrigin().getY(), cKMAXPOS));
		}

		// 物理オブジェクト関連
		// 座標の更新
		trans.setOrigin(trans.getOrigin() + cpuKeaperVel);

		rBodyCpuKeaper->getMotionState()->setWorldTransform(trans);

		// 描画オブジェクト関連
		// 座標のセット
		btVector3 physicsPos = trans.getOrigin();
		SetPosition(cpuKeaperPos, physicsPos);
	}
#pragma endregion
}

void GamePlay::SetPosition(XMFLOAT3& objPos, const btVector3& phyPos)
{
	objPos = XMFLOAT3(float(phyPos.getX()), float(phyPos.getY()), float(phyPos.getZ()));
}

void GamePlay::SetRotation(XMFLOAT3& objRot, const btVector3& phyRot)
{
	objRot = XMFLOAT3(float(phyRot.getX()), float(phyRot.getY()), float(phyRot.getZ()));
}

void GamePlay::ResetGame()
{
	ResetStatus(); // ゲーム内の状態のリセット

	// スコアの初期化
	myScore = 0;
	cpuScore = 0;
}

void GamePlay::ResetStatus()
{
	CreateBall(); // ボールを再生成

	CreateMyPlayer();
	CreateCpuPlayer();

	PhysicsUpdate();
	ObjectUpdate();

	kickoffPos = kickoffStartPos; // 初期位置にリセット

	goalCha->SetSize(goalStartSize.x, goalStartSize.y); // 初期サイズにリセット
	goalChaMag = 0.1f;
}

void GamePlay::ObjectUpdate()
{
	// カメラの更新
	Camera* camera = Camera::GetInstance();

	camera->Update();

#pragma region 味方 相手フィールドプレイヤーの更新
	for (int i = 0; i < PLAYERNUM; i++)
	{
		// 味方プレイヤ（体）
		objectMyBody[i]->SetPosition(myPlayerPos[i]);
		objectMyBody[i]->SetRotation(myPlayerAng[i]);
		objectMyBody[i]->Update(true);

		// 相手プレイヤ（体）
		objectCpuBody[i]->SetPosition(cpuPlayerPos[i]);
		objectCpuBody[i]->SetRotation(cpuPlayerAng[i]);
		objectCpuBody[i]->Update(true);
	}
#pragma endregion

#pragma region 味方ゴールキーパーの更新
	objectMyKeaper->SetPosition(myKeaperPos);
	objectMyKeaper->Update();
#pragma endregion

#pragma region 相手ゴールキーパーの更新
	objectCpuKeaper->SetPosition(cpuKeaperPos);
	objectCpuKeaper->Update();
#pragma endregion

#pragma region ボールの更新
	soccerBall->Update();
#pragma endregion

#pragma region スコアの更新
	// 味方
	myScoreNumber->SetRect(myScoreOrigin.x + myScore * myScoreSize.x, myScoreOrigin.y, myScoreSize.x, myScoreSize.y);

	// 相手
	cpuScoreNumber->SetRect(cpuScoreOrigin.x + cpuScore * cpuScoreSize.x, cpuScoreOrigin.y, cpuScoreSize.x, cpuScoreSize.y);

#pragma endregion

#pragma region スタジアムの更新
	// スタジアム
	stadium->Update();

	// 味方ゴール
	myGoal->Update();
	// 相手ゴール
	cpuGoal->Update();
#pragma endregion
}

void GamePlay::MyGoal()
{
	myScore++; // 味方スコア加算
}

void GamePlay::CpuGoal()
{
	cpuScore++; // 相手スコア加算
}