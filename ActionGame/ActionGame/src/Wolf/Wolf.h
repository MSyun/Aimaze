// オオカミ
// 2017. 1.26	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../Enemy/Enemy.h"
#include	"../AttackCollisionSpace/AttackCollisionSpace.h"


class Wolf	:	public	Enemy	{
private:
	bool	m_bAttack;
	bool	m_bInterval;
	float	m_fIntervalCnt;

	AttackCollisionSpace	m_HitSpace;

public:
	Wolf(int max);
	virtual ~Wolf();

private:
	virtual void Init();
	virtual void Update();

	void Action();
	virtual void MoveInput(const string cameraName);
	virtual void AttackInput();
};