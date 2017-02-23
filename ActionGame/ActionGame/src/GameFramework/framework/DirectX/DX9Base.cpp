//////////////////////////////////////////////////////////////
// 2016. 2. 7	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#include	"DX9Base.h"
#include	<tchar.h>
#include	"../Screen/Screen.h"
#include	"../Utility/System/SystemUtility.h"


/*									//
//			コンストラクタ			//
//									*/
DX9Base::DX9Base(Application* app) :
	IDXBase(app),
	m_bDeviceLost(false),
	m_hMenu(NULL),
	m_bActive(false),
	m_pGraphics(NULL)
{}


/*									//
//			デストラクタ			//
//									*/
DX9Base::~DX9Base() {
	Release();
}


/*									//
//				初期化				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT DX9Base::Init() {
	D3DDISPLAYMODE	d3ddm;

	// Direct3Dオブジェクト生成
	m_pD3Direct = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3Direct == NULL) {
		return E_FAIL;
	}

	// 現在のディスプレイモード取得
	if (FAILED(m_pD3Direct->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) {
		return E_FAIL;
	}

	PreWindow();
	PreScreen();

	if (!Screen::GetFullScreen())	m_D3Dpp = m_D3DppWindow;
	else							m_D3Dpp = m_D3DppFull;

	if (FAILED(CreateDevice()))
		return E_FAIL;

	if (FAILED(CreateViewport()))
		return E_FAIL;

	CreateState();

	ShowCursor(TRUE);

	if (FAILED(InitApp()))
		return E_FAIL;

	return S_OK;
}


/*									//
//		ウインドウモードの設定		//
//									*/
void DX9Base::PreWindow() {
	HWND hWnd = m_pApp->GetHWND();

	ZeroMemory( &m_D3DppWindow, sizeof(m_D3DppWindow) );
	m_D3DppWindow.BackBufferWidth				= 0;
	m_D3DppWindow.BackBufferHeight				= 0;
	m_D3DppWindow.BackBufferFormat				= D3DFMT_UNKNOWN;
	m_D3DppWindow.BackBufferCount				= 1;
	m_D3DppWindow.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_D3DppWindow.MultiSampleQuality			= 0;
	m_D3DppWindow.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_D3DppWindow.hDeviceWindow					= hWnd;
	m_D3DppWindow.Windowed						= TRUE;
	m_D3DppWindow.EnableAutoDepthStencil		= TRUE;								// ステンシルバッファ（TRUE:使用,FALSE:不使用）
	m_D3DppWindow.AutoDepthStencilFormat		= D3DFMT_D24S8;						// デプスとして24bit,ステンシルとして8bitを使う
	m_D3DppWindow.Flags							= 0;
	m_D3DppWindow.FullScreen_RefreshRateInHz	= 0;								// リフレッシュレート（指定できないので0固定）
	m_D3DppWindow.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル（VSyncを待たずに描画）
}


/*									//
//		フルスクリーンモードの設定	//
//									*/
void DX9Base::PreScreen() {
	HWND hWnd = m_pApp->GetHWND();

	ZeroMemory( &m_D3DppFull, sizeof(m_D3DppFull) );
	m_D3DppFull.BackBufferWidth				= Screen::GetWidth();
	m_D3DppFull.BackBufferHeight			= Screen::GetHeight();
	m_D3DppFull.BackBufferFormat			= D3DFMT_X8R8G8B8;
	m_D3DppFull.BackBufferCount				= 1;
	m_D3DppFull.MultiSampleType				= D3DMULTISAMPLE_NONE;
	m_D3DppFull.MultiSampleQuality			= 0;
	m_D3DppFull.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	m_D3DppFull.hDeviceWindow				= hWnd;
	m_D3DppFull.Windowed					= FALSE;
	m_D3DppFull.EnableAutoDepthStencil		= TRUE;							// ステンシルバッファ（TRUE:使用,FALSE:不使用）
	m_D3DppFull.AutoDepthStencilFormat		= D3DFMT_D24S8;					// デプスとして24bit,ステンシルとして8bitを使う
	m_D3DppFull.Flags						= 0;
	m_D3DppFull.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート（現在の速度に合わせる）
	m_D3DppFull.PresentationInterval		= D3DPRESENT_INTERVAL_DEFAULT;	// インターバル（VSyncを待って描画）
}


/*									//
//			デバイス設定			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT DX9Base::CreateDevice() {
	HWND hWnd = m_pApp->GetHWND();

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行う
	if (FAILED(m_pD3Direct->CreateDevice(
		D3DADAPTER_DEFAULT,						// ディスプレイアダプタ
		D3DDEVTYPE_HAL,							// デバイスタイプ
		hWnd,									// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING |	// デバイス作成制御の組み合わせ
		D3DCREATE_MULTITHREADED,				// マルチスレッドを安全に
		&m_D3Dpp,								// デバイスのプレゼンテーションパラメータ
		&m_pDevice)))							// デバイスインターフェースへのポインタ
	{
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行う
		if (FAILED(m_pD3Direct->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING |
			D3DCREATE_MULTITHREADED,
			&m_D3Dpp,
			&m_pDevice))) {
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行う
			if (FAILED(m_pD3Direct->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING |
				D3DCREATE_MULTITHREADED,
				&m_D3Dpp,
				&m_pDevice))) {
				return E_FAIL;
			}
		}
	}

	return S_OK;
}


/*									//
//			ビューポート設定		//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT DX9Base::CreateViewport() {
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = m_D3Dpp.BackBufferWidth;
	vp.Height = m_D3Dpp.BackBufferHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	HRESULT hr;
	hr = m_pDevice->SetViewport(&vp);
	if (FAILED(hr)) {
		HWND hWnd = m_pApp->GetHWND();
		MessageBox(hWnd, _T("ChangeWindowSize SetViewport"), _T("ビューポートの作成に失敗"), NULL);
		DestroyWindow(hWnd);
		return hr;
	}

	return S_OK;
}


/*									//
//			ステイト設定			//
//									*/
void DX9Base::CreateState() {
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);


	m_pGraphics = new Graphics;
	m_pGraphics->SetDevice(m_pDevice);

}


/*									//
//			アプリの初期化			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT DX9Base::InitApp() {
	return S_OK;
}


/*									//
//				ステップ			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT DX9Base::Step() {
	Update();
	Draw();

	return S_OK;
}


/*									//
//				更新				//
//									*/
void DX9Base::Update() {
}


/*									//
//				描画				//
//									*/
void DX9Base::Draw() {
	m_pDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pDevice->BeginScene())) {

		// Direct3Dによる描画の終了
		m_pDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}


/*									//
//				解放				//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT DX9Base::Release() {
	if (FAILED(ReleaseApp()))
		return E_FAIL;

	if (m_hMenu)
		DestroyMenu(m_hMenu);

	SAFE_DELETE(m_pGraphics);

	return S_OK;
}


/*									//
//			アプリの解放			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT DX9Base::ReleaseApp() {
	return S_OK;
}


/*									//
//			サイズの変更			//
//									*/
// <return> S_OK. 成功 : E_FAIL. 失敗
HRESULT DX9Base::ChangeWindowSize() {
	InvalidateDeviceObjects();

	HWND hWnd = m_pApp->GetHWND();

	// リセット
	HRESULT hr = m_pDevice->Reset(&m_D3Dpp);
	if (FAILED(hr)) {
		if (hr == D3DERR_DEVICELOST)
			m_bDeviceLost = true;
		else
			DestroyWindow(hWnd);
		MessageBox(hWnd, _T("ChangeWindowSize ERROR"), _T("ウインドウサイズの変更に失敗"), NULL);
		return hr;
	}

	// ビューポートの設定
	if (FAILED(CreateViewport()))
		return E_FAIL;

	// ステイトの初期化
	CreateState();

	RestoreDeviceObjects();

	return hr;
}


/*									//
//			モードの変更			//
//									*/
void DX9Base::ChangeDisplayMode() {
	HWND hWnd = m_pApp->GetHWND();
	bool window = !Screen::GetFullScreen();

	// モード変更
	window = !window;
	if (window) {
		m_D3Dpp = m_D3DppWindow;
	} else {
		m_D3Dpp = m_D3DppFull;
		GetWindowRect(hWnd, &m_rectWindow);
	}
	InvalidateDeviceObjects();

	HRESULT hr = m_pDevice->Reset(&m_D3Dpp);
	if (FAILED(hr)) {
		if (hr == D3DERR_DEVICELOST)
			m_bDeviceLost = true;
		else
			DestroyWindow(hWnd);
		MessageBox(hWnd, _T("ChangeDisplayMode Reset"), _T("ウインドウの変更に失敗しました"), NULL);
	}

	if (window) {
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		if (m_hMenu != NULL) {
			SetMenu(hWnd, m_hMenu);
			m_hMenu = NULL;
		}
		SetWindowPos(
			hWnd,
			HWND_NOTOPMOST,
			m_rectWindow.left,
			m_rectWindow.top,
			m_rectWindow.right - m_rectWindow.left,
			m_rectWindow.bottom - m_rectWindow.top,
			SWP_SHOWWINDOW);
	} else {
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		if (m_hMenu == NULL) {
			m_hMenu = GetMenu(hWnd);
			SetMenu(hWnd, NULL);
		}
	}
	// ステイトの初期化
	CreateState();

	RestoreDeviceObjects();
	m_pApp->SetHWND(hWnd);
}