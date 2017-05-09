// アプリケーション用ベース
// 2016. 2. 7	プログラム作成
// author		SyunMizuno


#pragma once

#include	<Windows.h>


class	Application {
public:
#pragma region method

	/* コンストラクタ
	// width	:	ウィンドウの横幅
	// height	:	ウィンドウの縦幅
	// bwnd		:	ウィンドウの確認
	*/
	Application(
		int width = 600,
		int height = 480,
		bool bwnd = true);

	/* デストラクタ */
	virtual ~Application();

	/* 実行
	// _hInstance	:	インスタンスハンドル
	*/
	int Run(HINSTANCE _hInstance);

	/* ローカル用プロシージャ
	// hWnd		:	ウィンドウハンドル
	// msg		:	メッセージ
	// wParam	:	パラメータ
	// lParam	:	パラメータ
	*/
	virtual LRESULT LocalWindowProc(
		HWND	hWnd,
		UINT	msg,
		WPARAM	wParam,
		LPARAM	lParam);

	/* プロシージャ
	// hWnd		:	ウィンドウハンドル
	// msg		:	メッセージ
	// wParam	:	パラメータ
	// lParam	:	パラメータ
	*/
	static LRESULT CALLBACK WndProc(
		HWND	hWnd,
		UINT	uMsg,
		WPARAM	wParam,
		LPARAM	lParam);

	// アプリケーションの終了
	static void End() { DestroyWindow(m_hWnd); }

#pragma region Getter

	HINSTANCE GetHInstance()			{ return m_hInstance; }
	void GetWndClass(WNDCLASSEX* wc)	{ *wc = m_WndClassEx; }
	//----- Window関連
	CREATESTRUCT GetWndCS()				{ return m_cs; }
	static HWND GetHWND()				{ return m_hWnd; }
	static void SetHWND(HWND _hwnd)		{ m_hWnd = _hwnd; }

#pragma endregion

#pragma endregion

#pragma region variable

protected:
	HINSTANCE		m_hInstance;	// アプリケーションハンドル
	WNDCLASSEX		m_WndClassEx;	// ウインドウクラス構造体
	//----- Window関連
	CREATESTRUCT	m_cs;
	static HWND m_hWnd;

#pragma endregion

#pragma region conceal method

private:
	// ウインドウクラスの登録
	virtual bool RegistWndClass();
	// メインウインドウの作成
	virtual bool CreateMainWnd();

protected:
	// ウインドウクラスの変更
	virtual void PreWndClass();
	// メインウインドウの変更
	virtual void PreMainWnd();
	// アプリケーションの初期化
	virtual bool Initialize();
	// メッセージループ
	virtual void Loop();
	// アプリケーション終了処理
	virtual void ReleaseApp();

#pragma endregion
};