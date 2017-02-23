//	�g�D�[���V�F�[�_
// 2016. 7.13	�v���O�����쐬
// Author		SyunMizuno

#pragma once

#include	"../HlslDrawBase.h"


/*
	memo:
		�G�b�W�ƃy�C���g�𕹗p���Ă��邽�ߕ��������̂��~�����ꍇ
		�����ō�鎖
*/

class	Toon : public	IHlslDrawBase {
private:
	D3DXHANDLE			m_hToonTex;
	LPDIRECT3DTEXTURE9	m_pTex;

public:
	Toon();
	virtual ~Toon();
	virtual HRESULT Load();
	void SetToonMap(LPDIRECT3DTEXTURE9 pTex);
	virtual void Begin();
};