//////////////////////////////////////////////////////////////
// DirectX9用クラス
// 2016. 2. 7	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once


#include	"DXBase.h"
#include	<d3dx9.h>
#include	"../Graphic/Graphics.h"


#if 1
#pragma comment ( lib, "d3d9.lib" )		// 描画処理に必要
#pragma comment ( lib, "d3dx9.lib" )	// [d3d9.lib]の拡張ライブラリ
#pragma comment ( lib, "dxguid.lib" )	// DirectXコンポーネント使用に必要
#endif

/*
	memo	:
		DirectX10 or 11で使うかわからないメンバもあるのでわかり次第べース含め変更
*/

/*								//
//		DirectX9用ベース		//
//								*/
class	DX9Base : public	IDXBase {
protected:
#pragma region variable

	D3DPRESENT_PARAMETERS	m_D3Dpp;		// 現在
	D3DPRESENT_PARAMETERS	m_D3DppWindow;	// ウインドウ・モード用
	D3DPRESENT_PARAMETERS	m_D3DppFull;	// フルスクリーン・モード用
	LPDIRECT3DDEVICE9		m_pDevice;		// 描画デバイス
	LPDIRECT3D9				m_pD3Direct;

	bool					m_bDeviceLost;	// デバイス消失フラグ
	RECT					m_rectWindow;	// ウインドウ・モードでの最後の位置
	HMENU					m_hMenu;		// メニュー・ハンドル
	bool					m_bActive;		// アクティブ状態

	Graphics*				m_pGraphics;	// グラフィック

#pragma endregion

public:
#pragma region method

	DX9Base(Application* app);
	virtual ~DX9Base();

	// 初期化
	virtual HRESULT Init();

	// ステップ関数
	virtual HRESULT Step();

	// 終了処理
	virtual HRESULT Release();

	virtual HRESULT ChangeWindowSize();	// サイズの変更
	virtual void ChangeDisplayMode();	// モードの変更

#pragma endregion

protected:
#pragma region conceal method

	virtual HRESULT InvalidateDeviceObjects() { return S_OK; };	// OnLostDevice
	virtual HRESULT RestoreDeviceObjects() { return S_OK; };	// OnResetDevice

	// モード設定
	virtual void PreWindow();	// ウインドウ
	virtual void PreScreen();	// フルスクリーン

	// ステイト設定
	virtual void CreateState();

	// ビューポート設定
	virtual HRESULT CreateViewport();

	// デバイスの設定
	virtual HRESULT CreateDevice();

	virtual HRESULT InitApp();
	virtual HRESULT ReleaseApp();
	virtual void Update();
	virtual void Draw();

#pragma endregion
};