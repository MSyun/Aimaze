// タイトルシーン
// 2017. 1. 1	プログラム作成
// author		SyunMizuno


#include	"GameScene.h"
#include	"../../GameFramework/framework/Debug/Debug.h"
#include	"../../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../../GameFramework/Obj/2D/Obj2DManager.h"
#include	"../../GameFramework/framework/Scene/SceneManagerWin.h"
#include	"../../GameFramework/framework/Mesh/SkinMesh/SkinMeshManager.h"
#include	"../../GameFramework/framework/Shader/ShaderManager.h"
#include	"../../GameFramework/framework/Camera/CameraManager.h"
#include	"../../GameFramework/framework/Texture/TexManager.h"
#include	"../../GameFramework/framework/Light/LightManager.h"
#include	"../../Archives/Archives.h"
#include	"../../Object/Weapon/Weapon.h"
#include	"../../GameFramework/framework/Utility/System/SystemUtility.h"
#include	"../../GameFramework/Sprite/Scaling/ScalingSprite.h"
#include	"../../GameFramework/framework/Screen/Screen.h"
#include	"../../GameFramework/Sprite/Moving/MovingSprite.h"
#include	"../../GameFramework/framework/Sound/SoundManager.h"

#include	"../../GameFramework/framework/Input/Input.h"
#include	"../../GameFramework/framework/Graphic/Graphics.h"
#include	"../../GameFramework/DrawManager/DrawManager.h"
#include	"../../Factory/Factory.h"

/*									//
//			コンストラクタ			//
//									*/
GameScene::GameScene() :
	m_pItemBoxManager(NULL)
{

}


/*									//
//			デストラクタ			//
//									*/
GameScene::~GameScene() {

}


/*									//
//				初期化				//
//									*/
bool GameScene::Init() {
	Debug::Log("ゲームシーン開始");

	GetCameraManager()->Create("Main");
	GetLightManager()->Create("Directional");

	GetSkinMeshManager()->Create(Archives::Mesh("Player"));
	GetSkinMeshManager()->Create(Archives::Mesh("Stage"));

	Obj3D* obj;
	// Player
	obj = (Obj3D*)Factory::Create(GAME_OBJ_PLAYER);
	obj->SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Player")), Factory::Create(GAME_ANIMATOR_PLAYER));
	obj->GetTransform()->SetPos(71.0f, 0.0f, -88.0f);
	obj->GetTransform()->Rotate(0.0f, -90.0f, 0.0f);
	LoadRate(10);

	// Stage
	obj = (Obj3D*)Factory::Create(GAME_OBJ_3D);
	obj->SetName("Stage");
	obj->SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Stage")));
	obj->SetTag(Archives::Tag("Stage"));
	obj->SetShaderType(SHADER_HALFLAMBERT);
	LoadRate(30);

	// CameraControll
	obj = (Obj3D*)Factory::Create(GAME_OBJ_CAMERACONTROLL);
	GetCameraManager()->Get("Main")->GetTransform()->SetParent(obj->GetTransform());
	obj->GetTransform()->Rotate(0.0f, -90.0f, 0.0f);

	m_pItemBoxManager = new ItemBoxManager;
	m_pItemBoxManager->Create(10);
	LoadRate(60);

	EnemyCreate();
	LoadRate(80);

	WeaponCreate();

	SpriteCreate();

	GetShaderManager()->Create(SHADER_HALFLAMBERT);
	GetShaderManager()->Create(SHADER_PHONG);
	ShadowMap* map = (ShadowMap*)GetShaderManager()->Create(SHADER_SHADOW);
	map->SetSize(Screen::GetWidth() / 3, Screen::GetHeight() / 3);
	Toon* toon = (Toon*)GetShaderManager()->Create(SHADER_TOON);
	toon->SetToonMap(GetTexManager()->Create(Archives::Texture("Toon")));
	LoadRate(100);

	//Sprite* sprite = new Sprite;
	//sprite->SetTexture(map->GetShadowTex());
	//sprite->GetRectTransform()->SetPos(Screen::GetWidth() / 5.0f, Screen::GetHeight() / 5.0f, 0.0f);

	// サウンド
	GetSoundManager()->Load(SOUND_BGM, Archives::Sound("Field").c_str(), "Field");
	GetSoundManager()->Play(SOUND_BGM, "Field");

	return true;
}


/*									//
//			読み込み率設定			//
//									*/
void GameScene::LoadRate(int rate) {
	if (GetSceneManagerWins())
		GetSceneManagerWins()->SetLoadRate(rate);
}


/*									//
//				敵作成				//
//									*/
void GameScene::EnemyCreate() {
	Obj3D* obj;

	obj = (Obj3D*)Factory::Create(GAME_OBJ_WIZARD, 10);
	obj->GetTransform()->SetPos(-16.0f, 0.0f, 5.0f);
	Factory::Create(GAME_OBJ_ALLOW);

	obj = (Obj3D*)Factory::Create(GAME_OBJ_WOLF, 10);
	obj->GetTransform()->SetPos(20.0f, 0.0f, -88.5f);
	obj = (Obj3D*)Factory::Create(GAME_OBJ_WOLF, 10);
	obj->GetTransform()->SetPos(76.5f, 0.0f, -35.0f);
	obj = (Obj3D*)Factory::Create(GAME_OBJ_WOLF, 10);
	obj->GetTransform()->SetPos(75.5f, 0.0f, 87.5f);
	obj = (Obj3D*)Factory::Create(GAME_OBJ_WOLF, 10);
	obj->GetTransform()->SetPos(16.0f, 0.0f, 82.0f);
}


/*									//
//			2Dオブジェクト作成		//
//									*/
void GameScene::SpriteCreate() {
	Vector2 size((float)Screen::GetWidth(), (float)Screen::GetHeight());
	// Start
	MovingSprite* sprite = (MovingSprite*)Factory::Create(GAME_OBJ_GAMESTART);
	sprite->SetTexture(GetTexManager()->Get(Archives::Texture("Start")));
	sprite->GetOnEndPlayReverse()->AddListener([=]() {
		Obj3D* obj = GetObj3DManager()->Find("Player");
		if (obj)
			obj->SetOperate(true);
		obj = GetObj3DManager()->Find("CameraControll");
		if (obj)
			obj->SetOperate(true);
		Factory::Create(GAME_OBJ_POSE);
	});

	// Mission
	ScalingSprite* mission;
	mission = (ScalingSprite*)Factory::Create(GAME_OBJ_MISSION);
	mission->SetTexture(GetTexManager()->Get(Archives::Texture("Mission")));
	mission->GetOnEndPlayReverse()->AddListener([=]() {
		sprite->Play(false);
	});
	GetSceneManagerWins()->GetFade()->GetOnBeginFadeOut()->AddListener([=]() {
		GetSceneManagerWins()->GetFade()->GetOnEndFadeIn()->AddListener([=]() {
			mission->Play();
		});
	});
}


/*									//
//				武器生成			//
//									*/
void GameScene::WeaponCreate() {
	Weapon* we;
	we = (Weapon*)Factory::Create(GAME_OBJ_SWORD);
	we->GetTransform()->SetPos(-15.f, 3.f, 27.5f);
	m_pItemBoxManager->RecordItem(we);

	we = (Weapon*)Factory::Create(GAME_OBJ_SWORD);
	we->GetTransform()->SetPos(50.f, 3.f, -88.f);
	m_pItemBoxManager->RecordItem(we);

	we = (Weapon*)Factory::Create(GAME_OBJ_SWORD);
	we->GetTransform()->SetPos(25.f, 3.f, -40.f);
	m_pItemBoxManager->RecordItem(we);
}


/*									//
//				解放				//
//									*/
void GameScene::Release() {
	//----- サウンド
	GetSoundManager()->Delete(SOUND_BGM, "Field");

	//GetShaderManager()->Delete(SHADER_HALFLAMBERT);
	//GetShaderManager()->Delete(SHADER_TOON);

	GetLightManager()->Delete("Directional");

	GetCameraManager()->Delete("Main");

	SAFE_DELETE(m_pItemBoxManager);

//	GetTexManager()->Delete(Archives::Texture("Toon"));

	GetSkinMeshManager()->Delete(Archives::Mesh("Stage"));

	GetObj3DManager()->AllClear();
	GetObj2DManager()->AllClear();
}


/*									//
//				更新				//
//									*/
void GameScene::Update() {
	Debug::Print("Scene : Game\n");

	// 一時的
	if (Input::GetKeyTrigger(DIK_1)) {
		GetSceneManagerWins()->ChangeCommand("Boss");
	}
	///////////

	GetObj3DManager()->Update();
	GetObj2DManager()->Update();
	GetObj3DManager()->LateUpdate();
	GetObj2DManager()->LateUpdate();
	GetObj3DManager()->CheckCollider();

	GetObj3DManager()->CheckDestroy();
	GetObj2DManager()->CheckDestroy();
}


/*									//
//				描画				//
//									*/
void GameScene::Draw() {
	GetLightManager()->AllSet();
	//GetDrawManager()->CreateShadow();

	GetCameraManager()->Set("Main");

	GetGraphics()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GetDrawManager()->Draw();

	GetGraphics()->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GetDrawManager()->LateDraw();

	GetObj2DManager()->Draw();
	GetObj2DManager()->LateDraw();
}


/*									//
//				再初期化			//
//									*/
bool GameScene::ReStart() {
	return true;
}