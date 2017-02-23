//	�N���X�t�B���^�[�N���X
// 2016. 9.29	�v���O�����쐬
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../HlslBase.h"


// �e�N�X�`�����W�̋�`
struct CoordRect {
	float u0, v0;
	float u1, v1;
};

class	CrossFilter : public	IHlslBase {
private:
#pragma region enum

	enum {
		NUM_STAR_TEXTURES = 8,	// ��䊂̃e�N�X�`���̖���
		MAX_SAMPLES = 16,		// �T���v���̍ő吔
	};

#pragma endregion

#pragma region variable

	// �o�b�t�@�̊�T�C�Y
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;
	// �k���o�b�t�@�̊�{�T�C�Y
	DWORD	m_dwCropWidth;
	DWORD	m_dwCropHeight;

	// �����̉��
	LPDIRECT3DSURFACE9		m_pOldBackBuffer;
	LPDIRECT3DSURFACE9		m_pOldZBuffer;
	// �ʏ��ʂ���������
	LPDIRECT3DSURFACE9		m_pMapZ;
	LPDIRECT3DTEXTURE9		m_pOriginalMap;
	LPDIRECT3DSURFACE9		m_pOriginalMapSurf;
	// �ʏ��ʂ̏k����
	LPDIRECT3DTEXTURE9		m_pTexSceneScaled;
	LPDIRECT3DSURFACE9		m_pSurfSceneScaled;
	// �P�x�̒��o
	LPDIRECT3DTEXTURE9		m_pTexBrightPass;
	LPDIRECT3DSURFACE9		m_pSurfBrightPass;
	// ���̂��ƂɂȂ�
	LPDIRECT3DTEXTURE9		m_pTexStarSource;
	LPDIRECT3DSURFACE9		m_pSurfStarSource;
	// ��
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

	// �e�N�X�`��
	LPDIRECT3DTEXTURE9 GetTexScene() { return m_pOriginalMap; }
	LPDIRECT3DTEXTURE9 GetTexScaled() { return m_pTexSceneScaled; }
	LPDIRECT3DTEXTURE9 GetTexBright() { return m_pTexBrightPass; }
	LPDIRECT3DTEXTURE9 GetTexStarSource() { return m_pTexStarSource; }
	LPDIRECT3DTEXTURE9 GetTexStar() { return m_apTexStar[0]; }

#pragma endregion

protected:
#pragma region conceal method

	// �e�N�X�`���E�T�[�t�F�X�̍쐬
	HRESULT CreateRenderTarget();

	// m_pTexScene��1/4�ɂ���m_pTexSceneScaled�ɓ����
	HRESULT Scene_To_SceneScaled();
	// �k���o�b�t�@�ɃR�s�[���ꂽ�����A���邢���������𒊏o����
	HRESULT SceneScaled_To_BrightPass();
	// �{�J�V��������
	HRESULT BrightPass_To_StarSource();
	// ��䊂̍쐬
	HRESULT RenderStar();

	// �u���[���|�������ɃK�E�X�^�̃{�J�V�ɂȂ�悤�ɌW�����v�Z����
	HRESULT GetGaussBlur5x5(
		DWORD dwD3DTexWidth,
		DWORD dwD3DTexHeight,
		D3DXVECTOR2* avTexCoordOffset,
		D3DXVECTOR4* avSampleWeight);

	HRESULT GetTextureCoords(
		LPDIRECT3DTEXTURE9 pTexSrc, RECT* pRectSrc,
		LPDIRECT3DTEXTURE9 pTexDst, RECT* pRectDest, CoordRect* pCoords);

	// �`�挳�A��̑傫������A�e�N�X�`�����W��␳����
	void DrawFullScreenQuad(float fLeftU, float fTopV, float fRightU, float fBottomV);

#pragma endregion
};