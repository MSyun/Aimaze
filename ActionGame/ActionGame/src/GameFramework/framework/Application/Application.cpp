//////////////////////////////////////////////////////////////
// 2016. 2. 7	プログラム作成
// 作成者		SyunMizuno
//////////////////////////////////////////////////////////////


#include	"Application.h"
#include	"../Screen/Screen.h"
#include	<tchar.h>
#include	<string>
using namespace std;

Application* pApp = NULL;

const LPCTSTR g_szClassName = _T("ClassName");

HWND Application::m_hWnd = NULL;


/*									//
//			コンストラクタ			//
//									*/
Application::Application(
	int width,
	int height,
	bool bwnd)
{
	pApp = this;
	Screen::SetSize(width, height);
	Screen::SetFullScreen(!bwnd);
}


/*									//
//			デストラクタ			//
//									*/
Application::~Application() {
}


/*									//
//			プロシージャ			//
//									*/
LRESULT CALLBACK Application::WndProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	return pApp->LocalWindowProc(hWnd, uMsg, wParam, lParam);
}


/*									//
//		ローカルプロシージャ		//
//									*/
LRESULT Application::LocalWindowProc(
	HWND	hWnd,
	UINT	uMsg,
	WPARAM	wParam,
	LPARAM	lParam)
{
	// デフォルトでは終了のみ
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	default:
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}


/*									//
//				実行				//
//									*/
int Application::Run(HINSTANCE _hInstance) {
	if (!_hInstance)	return false;
	m_hInstance = _hInstance;

	if (!RegistWndClass())	return false;
	if (!CreateMainWnd())	return false;
	if (!Initialize())		return false;

	Loop();

	ReleaseApp();

	return true;
}


/*									//
//		ウインドウクラスの登録		//
//									*/
// <return> true. 成功 : false. 失敗
bool Application::RegistWndClass() {
	PreWndClass();

	if( !RegisterClassEx( &m_WndClassEx ) ) {
		MessageBox( NULL, _T("Call to RegisterClassEx failed!"), _T("ウインドウクラスの作成に失敗"), NULL );
		return false;
	}

	return true;
}


/*									//
//		ウインドウクラスの変更		//
//									*/
void Application::PreWndClass() {
	m_WndClassEx.cbSize			= sizeof( WNDCLASSEX );				// 構造体のサイズ
	m_WndClassEx.style			= CS_HREDRAW | CS_VREDRAW;			// ウインドウクラスのスタイル
	m_WndClassEx.lpfnWndProc	= pApp->WndProc;					// ウインドウプロシージャを指定
	m_WndClassEx.cbClsExtra		= 0;								// 拡張用
	m_WndClassEx.cbWndExtra		= 0;								// 拡張用
	m_WndClassEx.hInstance		= m_hInstance;						// インスタンスハンドル
	m_WndClassEx.hIcon			= NULL;								// アイコンのハンドル
	m_WndClassEx.hCursor		= LoadCursor( NULL, IDC_ARROW );	// カーソルのハンドル
	m_WndClassEx.hbrBackground	= (HBRUSH)( COLOR_WINDOW + 1 );		// 背景ブラシのハンドル
	m_WndClassEx.lpszMenuName	= NULL;								// メニュー
	m_WndClassEx.lpszClassName	= g_szClassName;					// ウインドウクラスの名前
	m_WndClassEx.hIconSm		= NULL;								// 小さいアイコンのハンドル
}


/*									//
//		メインウインドウの作成		//
//									*/
// <return> true. 成功 : false. 失敗
bool Application::CreateMainWnd() {
	m_cs.hInstance = m_hInstance;
	m_cs.lpszClass = g_szClassName;

	PreMainWnd();

	m_hWnd = CreateWindowEx(
		m_cs.dwExStyle,
		m_cs.lpszClass,
		m_cs.lpszName,
		m_cs.style,
		m_cs.x,
		m_cs.y,
		m_cs.cx,
		m_cs.cy,
		m_cs.hwndParent,
		m_cs.hMenu,
		m_cs.hInstance,
		m_cs.lpCreateParams);

	if (!m_hWnd) {
		MessageBox(
			NULL,
			_T("Call to CreateWindow failed!"),
			_T("ウインドウハンドルの作成に失敗"),
			NULL);
		return false;
	}

	return true;
}


/*									//
//		メインウインドウの変更		//
//									*/
void Application::PreMainWnd() {
	RECT	rcRect;						// 画面中央座標取得用

	HWND hWnd = GetDesktopWindow();		// デスクトップのハンドルを取得
	GetWindowRect( hWnd, &rcRect );		// デスクトップの矩形を取得

	int width = Screen::GetWidth();
	int height = Screen::GetHeight();

	m_cs.dwExStyle		= 0;										// ウインドウの拡張スタイル
	m_cs.lpszName		= _T("Base");									// ウインドウ名
	m_cs.style			= WS_OVERLAPPEDWINDOW;						// Windowsスタイル
	m_cs.x				= (int)(rcRect.right * 0.5 - width * 0.5);		// 左
	m_cs.y				= (int)(rcRect.bottom * 0.5 - height * 0.5);		// 上
	m_cs.cx				= (width + GetSystemMetrics(SM_CXDLGFRAME) * 2 );
	m_cs.cy				= (height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION));
	m_cs.hwndParent		= NULL;			// 新しいウインドウを所有するウインドウ
	m_cs.hMenu			= NULL;			// 新しいウインドウが使用するメニュー
	m_cs.lpCreateParams	= NULL;			// ウインドウを作成するために使用するポインタ
}


/*									//
//		アプリケーションの初期化	//
//									*/
bool Application::Initialize() {
	if (!ShowWindow(m_hWnd, SW_SHOWNORMAL))
		return false;
	if (!UpdateWindow(m_hWnd))
		return false;

	return true;
}


/*									//
//			メッセージループ		//
//									*/
void Application::Loop() {
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


/*									//
//		アプリケーション終了処理	//
//									*/
void Application::ReleaseApp() {
	UnregisterClass(g_szClassName, m_WndClassEx.hInstance);
}