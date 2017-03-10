// 3Dゲージクラス
// 2017. 1.31	プログラム作成
// Author		SyunMizuno

#pragma once



#include	"../../../GameFramework/framework/Mesh/Quad/Quad.h"



class Gauge3	:	public	Quad {
protected:
	int		m_nMax;		// 最大値
	int		m_nCur;		// 現在値

public:
	Gauge3(int _max, LPDIRECT3DTEXTURE9 _tex = NULL);
	virtual ~Gauge3() {};

	virtual HRESULT Init(float _SizeX, float _SizeZ);

	void SetCurrentNum(int _num);
};