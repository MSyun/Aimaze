// ドラゴン用アニメーター
// 2017. 1.25	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../GameFramework/Animator/IAnimator.h"


class DragonAnimator	:	public	IAnimator {
private:
	bool	m_bWalk;
	int		m_nAttack;
	int		m_nRotate;
	trigger	m_tHit;
	trigger	m_tPlay;
	bool	m_bWing;
	trigger	m_tDeath;

public:
	DragonAnimator();
	virtual ~DragonAnimator();

private:
	virtual void ShiftTime();
	virtual void LoopTime();
	virtual void Loop();
	virtual void Period();
	virtual void State();
	virtual void StartState();

	virtual void AttackPack();
	virtual void RotatePack();
};