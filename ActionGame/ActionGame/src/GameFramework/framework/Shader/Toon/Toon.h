//	トゥーンシェーダ
// 2016. 7.13	プログラム作成
// Author		SyunMizuno

#pragma once

#include	"../HlslDrawBase.h"


/*
	memo:
		エッジとペイントを併用しているため分離したのが欲しい場合
		自分で作る事
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