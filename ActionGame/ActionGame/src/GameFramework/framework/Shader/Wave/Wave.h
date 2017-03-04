//	波シェーダ
// 2016. 7.15	プログラム作成
// Author		SyunMizuno

#pragma once


#include	"../HlslBase.h"


class	Wave : public	IHlslBase {
private:
#pragma region variable

	// 縦横比
	float MAP_WIDTH;
	float MAP_HEIGHT;

	D3DXHANDLE	m_hTexOffset;
	D3DXHANDLE	m_hSpringPower;
	D3DXHANDLE	m_hAddWavePos;
	D3DXHANDLE	m_hAddWaveHeight;
	D3DXHANDLE	m_hWaveTex;

	// 波マップ
	LPDIRECT3DTEXTURE9	m_pWaveMapTexture[2];
	LPDIRECT3DSURFACE9	m_pWaveMapSurface[2];

	// 法線マップ
	LPDIRECT3DTEXTURE9	m_pBumpMapTexture;
	LPDIRECT3DSURFACE9	m_pBumpMapSurface;

	// 波マップのインデックス
	int	m_nRenderTargetIndex;

#pragma endregion

public:
#pragma region method

	Wave();
	virtual ~Wave();
	virtual HRESULT Invalidate();
	virtual HRESULT Restore();
	virtual HRESULT Load();
	HRESULT SetSize(float x, float y);
	void SetSpringPower(float SpringPower);
	void AddWave(float posx, float posy, float height);

	LPDIRECT3DTEXTURE9 Render();

	// ハンドルの接続
	virtual void ConnectHandle();

#pragma endregion
};