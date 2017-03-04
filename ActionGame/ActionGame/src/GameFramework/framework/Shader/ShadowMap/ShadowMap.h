//	影用テクスチャクラス
// 2017. 2.26	プログラム作成
// author		SyunMizuno

#pragma once


#include	"../HlslDrawBase.h"



class ShadowMap	:	public	IHlslDrawBase {
private:
	UINT	m_unSizeX;
	UINT	m_unSizeY;

	// シャドウマップ
	LPDIRECT3DTEXTURE9	m_pZTex;			// テクスチャ
	LPDIRECT3DSURFACE9	m_pZSurf;			// サーフェス
	LPDIRECT3DSURFACE9	m_pZ;				// 深度バッファ
	LPDIRECT3DSURFACE9	m_pOldBackBuffer;	// サーフェス
	LPDIRECT3DSURFACE9	m_pOldZ;			// 深度バッファ

public:
	ShadowMap();
	virtual ~ShadowMap();

	virtual HRESULT Invalidate();
	virtual HRESULT Restore();
	virtual HRESULT Load();
	HRESULT SetSize(UINT x, UINT y);
	virtual void Begin();
	virtual void End();
	LPDIRECT3DTEXTURE9 GetShadowTex();

protected:
	HRESULT CreateRenderTarget();
	// ハンドルの接続
	virtual void ConnectHandle();
};