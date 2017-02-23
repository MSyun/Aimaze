// 魔法使い用アニメーター
// 2017. 1.24	プログラム作成
// Author		SyunMizuno


#pragma once



#include	"../GameFramework/Animator/IAnimator.h"


class	WizardAnimator	:	public	IAnimator {
private:
	bool	m_bWalk;
	trigger	m_tAttack;

public:
	WizardAnimator();
	virtual ~WizardAnimator();

private:
	virtual void ShiftTime();
	virtual void LoopTime();
	virtual void Loop();
	virtual void Period();
	virtual void State();
	virtual void StartState();
};