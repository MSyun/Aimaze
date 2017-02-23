// 2D�Q�[�W�N���X
// 2017. 2. 4	�v���O�����쐬
// Author		SyunMizuno

#pragma once


#include	"../GameFramework/Obj/2D/Sprite.h"


class Gauge2	:	public	Sprite {
protected:
	float	m_fMax;		// �ő�l
	float	m_fCur;		// �ŐV�l

	float	m_fTarget;	// �ړI�l

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