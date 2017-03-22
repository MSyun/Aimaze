// ゲーム用DirectX9
// 2016.12.29	プログラム作成
// author		SyunMizuno


#include	"GameDirectX9Base.h"
#include	<tchar.h>
#include	"../framework/Debug/Debug.h"
#include	"../framework/Input/Input.h"
#include	"../framework/Time/Time.h"
#include	"../framework/Save/SaveManagerBase.h"

#include	"../framework/Utility/System/SystemUtility.h"
#include	"../DelayFunction/Collection/DelayFunctionCollection.h"


/*									//
//			コンストラクタ			//
//									*/
GameDirectX9Base::GameDirectX9Base(Application* app) :
	DX9Base(app),
	m_pCurrentScene(NULL),
	m_pSceneManager(NULL),
	m_pTexManager(NULL),
	m_pMeshManager(NULL),
	m_pLightManager(NULL),
	m_pCameraManager(NULL),
	m_pSoundManager(NULL),
	m_pShaderManager(NULL),
	m_pObj3DManager(NULL),
	m_pObj2DManager(NULL),
	m_pControllerManager(NULL),
	m_pDrawManager(NULL)
{
}


/*									//
//			デストラクタ			//
//									*/
GameDirectX9Base::~GameDirectX9Base() {
	Release();
}


/*									//
//				ステップ			//
//									*/
HRESULT GameDirectX9Base::Step() {
	Time::Update();
	Input::Update();
	Update();
	Draw();

	return S_OK;
}


/*									//
//				解放				//
//									*/
HRESULT GameDirectX9Base::Release() {
	if (FAILED(ReleaseGame())) {
		Debug::LogError("ゲームの解放処理に失敗しました");
		return E_FAIL;
	}

	if (FAILED(DX9Base::Release())) {
		Debug::LogError("ゲームの解放処理に失敗しました");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//			アプリの解放			//
//									*/
HRESULT GameDirectX9Base::ReleaseApp() {
	SAFE_DELETE(m_pDrawManager);
	SAFE_DELETE(m_pSceneManager);
	SAFE_DELETE(m_pObj2DManager);
	SAFE_DELETE(m_pObj3DManager);
	SAFE_DELETE(m_pShaderManager);
	SAFE_DELETE(m_pSoundManager);
	SAFE_DELETE(m_pCameraManager);
	SAFE_DELETE(m_pLightManager);
	SAFE_DELETE(m_pMeshManager);
	SAFE_DELETE(m_pTexManager);
	SAFE_DELETE(m_pControllerManager);

	return S_OK;
}


/*									//
//				初期化				//
//									*/
HRESULT GameDirectX9Base::Init() {
	if (FAILED(DX9Base::Init())) {
		Debug::LogError("ゲームの初期化処理に失敗しました");
		return E_FAIL;
	}

	if (FAILED(InitGame())) {
		Debug::LogError("ゲームの初期化処理に失敗しました");
		return E_FAIL;
	}

	return S_OK;
}


/*									//
//			アプリの初期化			//
//									*/
HRESULT GameDirectX9Base::InitApp() {
	Debug::SetDevice(m_pDevice);
	Time::SetFrameRate(60);
	Input::Init(m_pApp->GetHWND());
	m_pObj3DManager = new Obj3DManager;
	m_pObj2DManager = new Obj2DManager;
	m_pTexManager = new TexManager;
	m_pMeshManager = new SkinMeshManager;
	m_pLightManager = new LightManager;
	m_pLightManager->SetCustom(true);	// シェーダ使用
	m_pCameraManager = new CameraManager;
	m_pSoundManager = new SoundManager;
	m_pSoundManager->Init(m_pApp->GetHWND());
	m_pShaderManager = new ShaderManager;
	m_pSceneManager = new SceneManagerWins;
	m_pSceneManager->SetLoadWait(10);
	m_pControllerManager = new GameControllerManager;
	m_pControllerManager->Create();
	m_pDrawManager = new DrawManager;
	m_pDrawManager->SetList(m_pObj3DManager);

	return S_OK;
}


/*									//
//				更新				//
//									*/
void GameDirectX9Base::Update() {
	m_pCurrentScene = m_pSceneManager->GetScene();
	if (m_pCurrentScene)
		m_pCurrentScene->Update();
	DelayFunctionCollection::Update();

	Debug::Print("\nFPS : ");
	Debug::Print(Time::GetFrameRate());
	Debug::Print("\n");
}


/*									//
//				描画				//
//									*/
void GameDirectX9Base::Draw() {
	m_pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pDevice->BeginScene())) {
		if (m_pCurrentScene)
			m_pCurrentScene->Draw();	// シーン
		m_pSceneManager->Draw();	// フェード

		Debug::Render();

		// Direct3Dによる描画の終了
		m_pDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}


/*									//
//			OnLostDevice			//
//									*/
HRESULT GameDirectX9Base::InvalidateDeviceObjects() {
	HRESULT hr = S_OK;

	hr = Debug::InvalidateDeviceObjects();
	if (FAILED(hr))	return hr;

	hr = m_pShaderManager->Invalidate();
	if (FAILED(hr))	return hr;

	hr = InvalidateDeviceObjectsGame();
	if (FAILED(hr))	return hr;

	return S_OK;
}


/*									//
//			OnResetDevice			//
//									*/
HRESULT GameDirectX9Base::RestoreDeviceObjects() {
	HRESULT hr;

	hr = Debug::RestoreDeviceObjects();
	if (FAILED(hr))	return hr;

	hr = m_pShaderManager->Restore();
	if (FAILED(hr))	return hr;

	hr = RestoreDeviceObjectsGame();
	if (FAILED(hr))	return hr;

	Debug::SetDevice(m_pDevice);
	GetGraphics()->SetDevice(m_pDevice);

	return S_OK;
}