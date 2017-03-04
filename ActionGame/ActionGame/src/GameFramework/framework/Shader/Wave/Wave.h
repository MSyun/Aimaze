//	�g�V�F�[�_
// 2016. 7.15	�v���O�����쐬
// Author		SyunMizuno

#pragma once


#include	"../HlslBase.h"


class	Wave : public	IHlslBase {
private:
#pragma region variable

	// �c����
	float MAP_WIDTH;
	float MAP_HEIGHT;

	D3DXHANDLE	m_hTexOffset;
	D3DXHANDLE	m_hSpringPower;
	D3DXHANDLE	m_hAddWavePos;
	D3DXHANDLE	m_hAddWaveHeight;
	D3DXHANDLE	m_hWaveTex;

	// �g�}�b�v
	LPDIRECT3DTEXTURE9	m_pWaveMapTexture[2];
	LPDIRECT3DSURFACE9	m_pWaveMapSurface[2];

	// �@���}�b�v
	LPDIRECT3DTEXTURE9	m_pBumpMapTexture;
	LPDIRECT3DSURFACE9	m_pBumpMapSurface;

	// �g�}�b�v�̃C���f�b�N�X
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

	// �n���h���̐ڑ�
	virtual void ConnectHandle();

#pragma endregion
};