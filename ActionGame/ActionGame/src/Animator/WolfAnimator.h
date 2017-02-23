// �T�p�A�j���[�^�[
// 2017. 1.24	�v���O�����쐬
// Author		SyunMizuno


#pragma once



#include	"../GameFramework/Animator/IAnimator.h"


class	WolfAnimator : public	IAnimator {
private:
	bool	m_bWalk;
	trigger	m_tAttack;

public:
	WolfAnimator();
	virtual ~WolfAnimator();

protected:
	virtual void ShiftTime();
	virtual void LoopTime();
	virtual void Loop();
	virtual void Period();
	virtual void State();
	virtual void StartState();
};