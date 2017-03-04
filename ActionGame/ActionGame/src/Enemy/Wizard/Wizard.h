// 魔法使い
// 2017. 1.26	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../Enemy.h"
#include	"../../Attack/AttackMagic/AttackMagic.h"
#include	"../../Ring/Ring.h"


class Wizard	:	public	Enemy {
private:
#pragma region variable
	bool	m_bAttack;
	bool	m_bInterval;
	float	m_fIntervalCnt;

	AttackMagic*	m_pShot;

	Ring*	m_pRing;


#pragma endregion

public:
#pragma region method

	Wizard(int max);
	virtual ~Wizard();

#pragma endregion

private:
#pragma region conceal method

	virtual void Init();
	virtual void Update();

	void Action();
	virtual void AttackInput();

#pragma endregion
};