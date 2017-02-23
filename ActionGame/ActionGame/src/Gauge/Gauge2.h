// 2Dゲージクラス
// 2017. 2. 4	プログラム作成
// Author		SyunMizuno

#pragma once


#include	"../GameFramework/Obj/2D/Sprite.h"


class Gauge2	:	public	Sprite {
protected:
	float	m_fMax;		// 最大値
	float	m_fCur;		// 最新値

	float	m_fTarget;	// 目的値

public:
	Gauge2(int _max = 100);
	virtual ~Gauge2();

	void SetMaxNum(float _max);
	void SetCurrentNum(float _num);
	void SetNum(float _num);
	bool CheckTarget() { return m_fTarget == m_fMax; }

protected:
	virtual void Update();

	virtual void SetVtx();
	virtual void SetTex();
};