//	�e�p�e�N�X�`���N���X
// 2017. 2.26	�v���O�����쐬
// author		SyunMizuno

#pragma once


#include	"../HlslDrawBase.h"



class ShadowMap	:	public	IHlslDrawBase {
private:
	UINT	m_unSizeX;
	UINT	m_unSizeY;

	// �V���h�E�}�b�v
	LPDIRECT3DTEXTURE9	m_pZTex;			// �e�N�X�`��
	LPDIRECT3DSURFACE9	m_pZSurf;			// �T�[�t�F�X
	LPDIRECT3DSURFACE9	m_pZ;				// �[�x�o�b�t�@
	LPDIRECT3DSURFACE9	m_pOldBackBuffer;	// �T�[�t�F�X
	LPDIRECT3DSURFACE9	m_pOldZ;			// �[�x�o�b�t�@

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
	// �n���h���̐ڑ�
	virtual void ConnectHandle();
};