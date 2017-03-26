// �^�C�g���V�[��
// 2017. 1. 1	�v���O�����쐬
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
//			�R���X�g���N�^			//
//									*/
ConfigScene::ConfigScene() {

}


/*									//
//			�f�X�g���N�^			//
//									*/
ConfigScene::~ConfigScene() {

}


/*									//
//				������				//
//									*/
bool ConfigScene::Init() {
	return true;
}


/*									//
//				���				//
//									*/
void ConfigScene::Release() {
}


/*									//
//				�X�V				//
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
//				�`��				//
//									*/
void ConfigScene::Draw() {

	GetGraphics()->SetAlphaBlendEnable(false);
	GetDrawManager()->Draw();

	GetGraphics()->SetAlphaBlendEnable(true);
	GetDrawManager()->LateDraw();

	GetObj2DManager()->Draw();
	GetObj2DManager()->LateDraw();
}