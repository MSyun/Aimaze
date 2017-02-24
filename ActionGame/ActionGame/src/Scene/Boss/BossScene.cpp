// ボスシーン
// 2017. 2. 6	プログラム作成
// author		SyunMizuno



#include	"BossScene.h"
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
#include	"../../Player/Player.h"
#include	"../../Dragon/Dragon.h"

#include	"../../Animator/PlayerAnimator.h"
#include	"../../Animator/DragonAnimator.h"
#include	"../../GameFramework/framework/Utility/System/SystemUtility.h"
#include	"../../CameraControll/CameraControll.h"
#include	"../../Pose/Pose.h"
#include	"../../GameFramework/framework/Graphic/Graphics.h"
#include	"../../GameFramework/DrawManager/DrawManager.h"



/*									//
//			コンストラクタ			//
//									*/
BossScene::BossScene() :
	m_pItemBoxManager(NULL)
{

}


/*									//
//			デストラクタ			//
//									*/
BossScene::~BossScene() {

}


/*									//
//				初期化				//
//									*/
bool BossScene::Init() {
	Debug::Log("ボスシーン開始");

	Camera* camera = GetCameraManager()->Create("Main");
	GetLightManager()->Create("Directional");
	GetSkinMeshManager()->Create(Archives::Mesh("Player"));
	GetSkinMeshManager()->Create(Archives::Mesh("Dragon"));
	GetSkinMeshManager()->Create(Archives::Mesh("Land"));

	Obj3D* obj;
	// Player
	obj = new Player;
	obj->ChangeOperate();
	obj->SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Player")), new PlayerAnimator);
	obj->GetTransform()->SetPos(0.0f, 0.0f, -60.0f);
	LoadRate(10);

	// Stage
	obj = new Obj3D;
	obj->SetName("Stage");
	obj->SetShaderType(SHADER_HALFLAMBERT);
	obj->SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Land")));
	obj->SetTag(Archives::Tag("Stage"));
	obj->GetTransform()->SetScale(20.0f, 20.0f, 20.0f);
	LoadRate(30);

	// CameraControll
	obj = new CameraControll;
	camera->GetTransform()->SetParent(obj->GetTransform());
	obj->ChangeOperate();
	obj->GetTransform()->Rotate(0.0f, 0.0f, 0.0f);

	EnemyCreate();
	LoadRate(60);

	m_pItemBoxManager = new ItemBoxManager;
	m_pItemBoxManager->Create(10);
	LoadRate(80);

	GetShaderManager()->Create(SHADER_HALFLAMBERT);
	GetShaderManager()->Create(SHADER_PHONG);
	Toon* toon = (Toon*)GetShaderManager()->Create(SHADER_TOON);
	toon->SetToonMap(GetTexManager()->Create(Archives::Texture("Toon")));
	LoadRate(100);

	new Pose;

	return true;
}


/*									//
//				解放				//
//									*/
void BossScene::Release() {
	SAFE_DELETE(m_pItemBoxManager);

	GetShaderManager()->Delete(SHADER_HALFLAMBERT);
	GetShaderManager()->Delete(SHADER_TOON);
	GetShaderManager()->Delete(SHADER_PHONG);

	GetSkinMeshManager()->Delete(Archives::Mesh("Player"));
	GetSkinMeshManager()->Delete(Archives::Mesh("Dragon"));
	GetSkinMeshManager()->Delete(Archives::Mesh("Land"));

	GetCameraManager()->Delete("Main");

	GetObj3DManager()->AllClear();
	GetObj2DManager()->AllClear();
}


/*									//
//			読み込み率設定			//
//									*/
void BossScene::LoadRate(int rate) {
	if (GetSceneManagerWins()) {
		GetSceneManagerWins()->SetLoadRate(rate);
	}
}


/*									//
//				敵生成				//
//									*/
void BossScene::EnemyCreate() {
	Obj3D* obj;

	obj = new Dragon(30);
	obj->SetModel(GetSkinMeshManager()->Get(Archives::Mesh("Dragon")), new DragonAnimator);
}


/*									//
//				更新				//
//									*/
void BossScene::Update() {
	Debug::Print("Scene : Boss\n");

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
void BossScene::Draw() {
	GetLightManager()->AllSet();

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
bool BossScene::ReStart() {
	return true;
}