//	クロスフィルタークラス
// 2016. 9.29	プログラム作成
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../HlslBase.h"


// テクスチャ座標の矩形
struct CoordRect {
	float u0, v0;
	float u1, v1;
};

class	CrossFilter : public	IHlslBase {
private:
#pragma region enum

	enum {
		NUM_STAR_TEXTURES = 8,	// 光芒のテクスチャの枚数
		MAX_SAMPLES = 16,		// サンプラの最大数
	};

#pragma endregion

#pragma region variable

	// バッファの基準サイズ
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	// 縮小バッファの基本サイズ
	DWORD	m_dwCropWidth;
	DWORD	m_dwCropHeight;

	// 初期の画面
	LPDIRECT3DSURFACE9		m_pOldBackBuffer;
	LPDIRECT3DSURFACE9		m_pOldZBuffer;
	// 通常画面を書き込む
	LPDIRECT3DSURFACE9		m_pMapZ;
	LPDIRECT3DTEXTURE9		m_pOriginalMap;
	LPDIRECT3DSURFACE9		m_pOriginalMapSurf;
	// 通常画面の縮小版
	LPDIRECT3DTEXTURE9		m_pTexSceneScaled;
	LPDIRECT3DSURFACE9		m_pSurfSceneScaled;
	// 輝度の抽出
	LPDIRECT3DTEXTURE9		m_pTexBrightPass;
	LPDIRECT3DSURFACE9		m_pSurfBrightPass;
	// 星のもとになる
	LPDIRECT3DTEXTURE9		m_pTexStarSource;
	LPDIRECT3DSURFACE9		m_pSurfStarSource;
	// 星
	LPDIRECT3DTEXTURE9		m_apTexStar[NUM_STAR_TEXTURES];
	LPDIRECT3DSURFACE9		m_apSurfStar[NUM_STAR_TEXTURES];

#pragma endregion

public:
#pragma region method

	CrossFilter();
	virtual ~CrossFilter();

	virtual HRESULT Invalidate();
	virtual HRESULT Restore();
	virtual HRESULT Load();
	HRESULT SetSize(float x, float y);
	virtual void Begin();
	virtual void End();

	// テクスチャ
	LPDIRECT3DTEXTURE9 GetTexScene() { return m_pOriginalMap; }
	LPDIRECT3DTEXTURE9 GetTexScaled() { return m_pTexSceneScaled; }
	LPDIRECT3DTEXTURE9 GetTexBright() { return m_pTexBrightPass; }
	LPDIRECT3DTEXTURE9 GetTexStarSource() { return m_pTexStarSource; }
	LPDIRECT3DTEXTURE9 GetTexStar() { return m_apTexStar[0]; }

#pragma endregion

protected:
#pragma region conceal method

	// テクスチャ・サーフェスの作成
	HRESULT CreateRenderTarget();

	// m_pTexSceneを1/4にしてm_pTexSceneScaledに入れる
	HRESULT Scene_To_SceneScaled();
	// 縮小バッファにコピーされたうち、明るい部分だけを抽出する
	HRESULT SceneScaled_To_BrightPass();
	// ボカシをかける
	HRESULT BrightPass_To_StarSource();
	// 光芒の作成
	HRESULT RenderStar();

	// ブラーを掛けた時にガウス型のボカシになるように係数を計算する
	HRESULT GetGaussBlur5x5(
		DWORD dwD3DTexWidth,
		DWORD dwD3DTexHeight,
		D3DXVECTOR2* avTexCoordOffset,
		D3DXVECTOR4* avSampleWeight);

	HRESULT GetTextureCoords(
		LPDIRECT3DTEXTURE9 pTexSrc, RECT* pRectSrc,
		LPDIRECT3DTEXTURE9 pTexDst, RECT* pRectDest, CoordRect* pCoords);

	// 描画元、先の大きさから、テクスチャ座標を補正する
	void DrawFullScreenQuad(float fLeftU, float fTopV, float fRightU, float fBottomV);

#pragma endregion
};