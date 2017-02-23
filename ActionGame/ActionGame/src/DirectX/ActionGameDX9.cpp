// アクションゲーム用DirectX9
// 2017. 1. 1	プログラム作成
// author		SyunMizuno


#include	"ActionGameDX9.h"

#include	"../Scene/TitleLogo/TitleLogoScene.h"
#include	"../Scene/Title/TitleScene.h"
#include	"../Scene/Game/GameScene.h"
#include	"../Scene/Config/ConfigScene.h"
#include	"../Scene/Load/LoadScene.h"
#include	"../Scene/Boss/BossScene.h"
#include	"../GameFramework/framework/Screen/Screen.h"


/*									//
//			コンストラクタ			//
//									*/
ActionGameDX9::ActionGameDX9(Application* pApp) :
	GameDirectX9Base(pApp)
{
}


/*									//
//			デストラクタ			//
//									*/
ActionGameDX9::~ActionGameDX9() {
}


/*									//
//			ゲームの初期化			//
//									*/
HRESULT ActionGameDX9::InitGame() {
	m_pSceneManager->RegistSceneMakeInstance<TitleLogoScene>("TitleLogo");
	m_pSceneManager->RegistSceneMakeInstance<TitleScene>("Title");
	m_pSceneManager->RegistSceneMakeInstance<ConfigScene>("Config");
	m_pSceneManager->RegistSceneMakeInstance<GameScene>("Game");
	m_pSceneManager->RegistSceneMakeInstance<BossScene>("Boss");
	m_pSceneManager->RegistLoadingSceneMakeInstance<LoadScene>();
	m_pSceneManager->RegistFadeScene(new SceneFadeBase(
		m_pDevice,
		true,
		Screen::GetWidth(),
		Screen::GetHeight(),
		1.0f));
	GetSceneManagerWins()->PushCommand("Title");

	return S_OK;
}


/*									//
//			ゲームの解放			//
//									*/
HRESULT ActionGameDX9::ReleaseGame() {
	return S_OK;
}


/*									//
//			OnLostDevice			//
//									*/
HRESULT ActionGameDX9::InvalidateDeviceObjectsGame() {
	return S_OK;
}


/*									//
//			OnResetDevice			//
//									*/
HRESULT ActionGameDX9::RestoreDeviceObjectsGame() {
	return S_OK;
}