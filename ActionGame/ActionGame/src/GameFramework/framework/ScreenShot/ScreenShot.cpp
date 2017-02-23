// スクリーンショットクラス
// 2015.11.14	プロジェクト作成
// author		SyunMizuno

#include	"ScreenShot.h"


/*
	MEMO	:
			違うスレッドで実行した方が処理落ち等がなくなってよいかも
*/


/*									//
//			コンストラクタ			//
//									*/
ScreenShot::ScreenShot(IDirect3DDevice9* pDevice, HWND hWnd) :
	m_pDevice(pDevice),
	m_hWnd(hWnd)
{}


/*									//
//			デストラクタ			//
//									*/
ScreenShot::~ScreenShot() {
}


/*									//
//				撮影				//
//									*/
void ScreenShot::Snap(const string filename, bool window) {
	IDirect3DSurface9*	pSurface = NULL;

	this->CreateSurface(pSurface);

	RECT rect = this->CreateRect(window);

	// サーフェイスを画像に出力
	D3DXSaveSurfaceToFile(
		filename.c_str(),	// 出力するファイル名
		D3DXIFF_BMP,		// 出力するファイル形式BITMAPとか
		pSurface,			// 入力するサーフェイス
		NULL,				// 256 色のパレットを含む PALETTEENTRY 構造体へのポインタ　通常はNULL
		&rect);				// 出力する画像の範囲 NULLだと画像全体

	pSurface->Release();
	pSurface = NULL;
}


/*									//
//			サーフェイスの作成		//
//									*/
void ScreenShot::CreateSurface(IDirect3DSurface9* pSurface) {
	m_pDevice->CreateOffscreenPlainSurface(
		GetSystemMetrics(SM_CXSCREEN),	// サーフェイスの幅
		GetSystemMetrics(SM_CYSCREEN),	// サーフェイスの高さ
		D3DFMT_A8R8G8B8,				// プライマリバッファと同じD3DFORMAT
		D3DPOOL_SYSTEMMEM,				// メモリのアクセス形式
		&pSurface,						// 作成するサーフェイス
		NULL);							// 予約されているもの	通常はNULL

	m_pDevice->GetFrontBufferData(
		0,				// 通常は0
		pSurface);		// サーフェイスにデータを出力
}


/*									//
//		画面サイズの矩形を作成		//
//									*/
RECT ScreenShot::CreateRect(bool window) {
	RECT rect;
	GetWindowRect(m_hWnd, &rect);

	// ウインドウの場合
	if (window) {// ウインドウサイズに合わせる
		rect.bottom -= GetSystemMetrics(SM_CYDLGFRAME);
		rect.top += GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYCAPTION);
		rect.left += GetSystemMetrics(SM_CXDLGFRAME);
		rect.right -= GetSystemMetrics(SM_CXDLGFRAME);
	}

	return rect;
}