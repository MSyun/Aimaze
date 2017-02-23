// 頭上のライフ
// 2017. 1.31	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../Gauge/Gauge3.h"


class LifeberOnHead	:	public	Obj3D {
protected:
	Gauge3* m_pGauge;
	int	m_nCurLife;

public:
	LifeberOnHead(int max);
	virtual ~LifeberOnHead();

	void SetCurLife(int life) { m_pGauge->SetCurrentNum(life); }
};