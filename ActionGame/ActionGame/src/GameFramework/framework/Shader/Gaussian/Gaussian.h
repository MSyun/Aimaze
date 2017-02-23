//	ガウスフィルタクラス
// 2016. 4.25	プログラム作成
// 2016. 7. 6	ベースを継承、シングルトンを排除
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../HlslBase.h"

/*
	memo:
		重みの計算をCPUで行っている
		GPUで行ったほうが良い場合はSheder内で計算するようにする
		受け取った画像をぼかすだけのクラスに特化してもよいかも
*/

class	Gaussian : public	IHlslBase {
private:
#pragma region variable

	// 縦横比
	float MAP_WIDTH;
	float MAP_HEIGHT;
#define	WEIGHT_MUN	8	// 重み計算をする個数

	D3DXHANDLE			m_hWeight;				// 重みの配列
	D3DXHANDLE			m_hSrcMap;				// テクスチャ
	float				m_fWeight[WEIGHT_MUN];	// 重み
	float				m_fDispersion_sq;		// 分散の平方根

	// 初期の状態
	LPDIRECT3DSURFACE9	m_pOldBackBuffer;
	LPDIRECT3DSURFACE9	m_pOldZBuffer;
	// 通常画面を書き込む
	LPDIRECT3DSURFACE9	m_pMapZ;		// 深度バッファ
	LPDIRECT3DTEXTURE9	m_pOriginalMap;	// テクスチャ
	LPDIRECT3DSURFACE9	m_pOriginalMapSurf;
	// X方向にぼかした画面
	LPDIRECT3DTEXTURE9	m_pXMap;
	LPDIRECT3DSURFACE9	m_pXMapSurf;
	// XY方向にぼかした画面
	LPDIRECT3DTEXTURE9	m_pXYMap;
	LPDIRECT3DSURFACE9	m_pXYMapSurf;

#pragma endregion

public:
#pragma region method

	Gaussian();
	virtual ~Gaussian();

	virtual HRESULT Invalidate();
	virtual HRESULT Restore();
	virtual HRESULT Load();
	HRESULT SetSize(float x, float y);
	virtual void Begin();
	virtual void End();
	void UpdateWeight(float dispersion);

	// テクスチャ取得（デバッグとかに使用、XYMapは最終的な画像
	LPDIRECT3DTEXTURE9 GetOriginalMap() { return m_pOriginalMap; }
	LPDIRECT3DTEXTURE9 GetXMap() { return m_pXMap; }
	LPDIRECT3DTEXTURE9 GetXYMap() { return m_pXYMap; }

#pragma endregion

protected:
#pragma region conceal method

	// テクスチャ・サーフェスの作成
	HRESULT CreateRenderTarget();

#pragma endregion
};