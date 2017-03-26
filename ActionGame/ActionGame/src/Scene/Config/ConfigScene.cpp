// タイトルシーン
// 2017. 1. 1	プログラム作成
// author		SyunMizuno


#include	"ConfigScene.h"
#include	"../../GameFramework/framework/Debug/Debug.h"
#include	"../../GameFramework/Obj/2D/Obj2DManager.h"
#include	"../../GameFramework/Obj/3D/Obj3DManager.h"
#include	"../../GameFramework/GameController/GameControllerManager.h"
#include	"../../GameFramework/framework/Scene/SceneManagerWin.h"
#include	"../../GameFramework/framework/Graphic/Graphics.h"
#include	"../../GameFramework/DrawManager/DrawManager.h"



/*									//
//			コンストラクタ			//
//									*/
ConfigScene::ConfigScene() {

}


/*									//
//			デストラクタ			//
//									*/
ConfigScene::~ConfigScene() {

}


/*									//
//				初期化				//
//									*/
bool ConfigScene::Init() {
	return true;
}


/*									//
//				解放				//
//									*/
void ConfigScene::Release() {
}


/*									//
//				更新				//
//									*/
void ConfigScene::Update() {
	Debug::Print("Scene : Config\n");

	GetObj3DManager()->Update();
	GetObj2DManager()->Update();
	GetObj3DManager()->LateUpdate();
	GetObj2DManager()->LateUpdate();
	GetObj3DManager()->CheckCollider();

	GetObj3DManager()->CheckDestroy();
	GetObj2DManager()->CheckDestroy();

	if (Input::GetKeyTrigger(GetContManager()->Get()->GetKeyNum(BUTTON_MARU)) ||
		Input::GetJoyTrigger(0, GetContManager()->Get()->GetJoyNum(BUTTON_MARU))) {
		GetSceneManagerWins()->PopCommand();
	}
}


/*									//
//				描画				//
//									*/
void ConfigScene::Draw() {

	GetGraphics()->SetAlphaBlendEnable(false);
	GetDrawManager()->Draw();

	GetGraphics()->SetAlphaBlendEnable(true);
	GetDrawManager()->LateDraw();

	GetObj2DManager()->Draw();
	GetObj2DManager()->LateDraw();
}