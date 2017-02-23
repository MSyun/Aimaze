//	�K�E�X�t�B���^�N���X
// 2016. 4.25	�v���O�����쐬
// 2016. 7. 6	�x�[�X���p���A�V���O���g����r��
// author		SyunMizuno

#pragma once


#include	<d3dx9.h>
#include	"../HlslBase.h"

/*
	memo:
		�d�݂̌v�Z��CPU�ōs���Ă���
		GPU�ōs�����ق����ǂ��ꍇ��Sheder���Ōv�Z����悤�ɂ���
		�󂯎�����摜���ڂ��������̃N���X�ɓ������Ă��悢����
*/

class	Gaussian : public	IHlslBase {
private:
#pragma region variable

	// �c����
	float MAP_WIDTH;
	float MAP_HEIGHT;
#define	WEIGHT_MUN	8	// �d�݌v�Z�������

	D3DXHANDLE			m_hWeight;				// �d�݂̔z��
	D3DXHANDLE			m_hSrcMap;				// �e�N�X�`��
	float				m_fWeight[WEIGHT_MUN];	// �d��
	float				m_fDispersion_sq;		// ���U�̕�����

	// �����̏��
	LPDIRECT3DSURFACE9	m_pOldBackBuffer;
	LPDIRECT3DSURFACE9	m_pOldZBuffer;
	// �ʏ��ʂ���������
	LPDIRECT3DSURFACE9	m_pMapZ;		// �[�x�o�b�t�@
	LPDIRECT3DTEXTURE9	m_pOriginalMap;	// �e�N�X�`��
	LPDIRECT3DSURFACE9	m_pOriginalMapSurf;
	// X�����ɂڂ��������
	LPDIRECT3DTEXTURE9	m_pXMap;
	LPDIRECT3DSURFACE9	m_pXMapSurf;
	// XY�����ɂڂ��������
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

	// �e�N�X�`���擾�i�f�o�b�O�Ƃ��Ɏg�p�AXYMap�͍ŏI�I�ȉ摜
	LPDIRECT3DTEXTURE9 GetOriginalMap() { return m_pOriginalMap; }
	LPDIRECT3DTEXTURE9 GetXMap() { return m_pXMap; }
	LPDIRECT3DTEXTURE9 GetXYMap() { return m_pXYMap; }

#pragma endregion

protected:
#pragma region conceal method

	// �e�N�X�`���E�T�[�t�F�X�̍쐬
	HRESULT CreateRenderTarget();

#pragma endregion
};