// プレイヤー用アニメーター
// 2017. 1.23	プログラム作成
// Author		SyunMizuno



#pragma once


#include	"../GameFramework/Animator/IAnimator.h"


class	PlayerAnimator	:	public	IAnimator {
private:

	bool		m_bWeapon;
	bool		m_bRun;
	trigger		m_tHit;
	trigger		m_tAttack;
	trigger		m_tDeath;

public:
	PlayerAnimator();
	virtual ~PlayerAnimator();

protected:
	virtual void ShiftTime();
	virtual void LoopTime();
	virtual void Loop();
	virtual void Period();
	virtual void State();
	virtual void StartState();
};