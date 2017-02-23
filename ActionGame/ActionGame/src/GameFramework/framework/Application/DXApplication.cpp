//////////////////////////////////////////////////////////////
// 2016. 2. 7	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#include	<tchar.h>
#include	"DXApplication.h"
#include	"../Screen/Screen.h"


/*									//
//			コンストラクタ			//
//									*/
DXApplication::DXApplication(int width, int height, bool bwnd) :
	Application(width, height, bwnd),
	m_pDXObj(NULL)
{}


/*									//
//			デストラクタ			//
//									*/
DXApplication::~DXApplication() {
}


/*									//
//				変更				//
//									*/
void DXApplication::PreMainWnd() {
	RECT	rcRect;						// 画面中央座標取得用

	HWND hwnd;
	hwnd = GetDesktopWindow();		// デスクトップのハンドルを取得
	GetWindowRect(hwnd, &rcRect );	// デスクトップの矩形を取得
	SetHWND(hwnd);

	int width = Screen::GetWidth();
	int height = Screen::GetHeight();

	m_cs.dwExStyle		= 0;
	m_cs.lpszName		= _T("DXBase");
	m_cs.style			= !Screen::GetFullScreen() ? WS_OVERLAPPEDWINDOW | WS_VISIBLE : WS_POPUP | WS_VISIBLE;
	m_cs.x				= rcRect.right / 2 - width / 2;
	m_cs.y				= rcRect.bottom / 2 - height / 2;
	m_cs.cx				= (width + GetSystemMetrics(SM_CXDLGFRAME) * 2 );
	m_cs.cy				= (height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION));
	m_cs.hwndParent		= NULL;
	m_cs.hMenu			= NULL;
	m_cs.lpCreateParams	= NULL;
}


/*									//
//		アプリケーションの初期化	//
//									*/
// <return> true. 成功 : false. 失敗
bool DXApplication::Initialize() {
	if (m_pDXObj) {
		if (FAILED(m_pDXObj->Init()))
			return false;
	}

	// ウインドウ表示
	if (!Application::Initialize())
		return false;

	return true;
}


/*									//
//			メッセージループ		//
//									*/
void DXApplication::Loop() {
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	if (!m_pDXObj)
		return;

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if (FAILED(m_pDXObj->Step()))
				break;
		}
	}
}


/*									//
//		アプリケーション終了処理	//
//									*/
void DXApplication::ReleaseApp() {
	if (m_pDXObj) {
		delete m_pDXObj;
		m_pDXObj = NULL;
	}

	Application::ReleaseApp();
}


/*									//
//		ウインドウプロシージャ		//
//									*/
LRESULT DXApplication::LocalWindowProc(
	HWND	hWnd,
	UINT	msg,
	WPARAM	wParam,
	LPARAM	lParam)
{
	// デフォルトでは終了のみ
	switch (msg) {
		//----- キー入力関係
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

		//----- Alt + 特殊キー
	case WM_SYSKEYDOWN:
		switch (wParam) {
		case VK_RETURN:	// Alt + Enterを押すと切り替え
//			m_pDXObj->ChangeDisplayMode();
			break;

		case VK_SHIFT:
			//			m_pSnap->Snap( "Snap.png", m_bWindow );

		default:
			break;
		}

		//----- サイズ変更
	case WM_SIZE:
		//if (m_D3Dpp.Windowed != TRUE)
		//	break;

		//if (!m_pD3DDevice || wParam == SIZE_MINIMIZED)
		//	break;
		//m_D3Dpp.BackBufferWidth  = LOWORD(lParam);
		//m_D3Dpp.BackBufferHeight = HIWORD(lParam);
		//if(m_bDeviceLost)
		//	break;
		//if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
		//	ChangeWindowSize();
		break;

		//----- デストロ～イ
	case WM_DESTROY:
		// DirectX Graphicsの終了処理
		PostQuitMessage(0);
		m_hWnd = NULL;
		break;


	case WM_ACTIVATE:
		//		m_bActive = (LOWORD(wParam) != 0 );
		break;

	default:
		return Application::LocalWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	return 0;
}