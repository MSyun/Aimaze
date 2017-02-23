// �G
// 2017. 1.31	�v���O�����쐬
// Author		SyunMizuno

#pragma once


#include	"../Character/Charcter.h"
#include	"../LifeberOnHead/LifeberOnHead.h"
#include	"../DeathEffect/DeathEffect.h"


class Enemy	:	public	Character {
protected:
	Obj3D*	m_pPlayer;
	LifeberOnHead*	m_pLifeber;
	DeathEffect*	m_pDeathEffect;

public:
	Enemy(int max);
	virtual ~Enemy();

	// �_���[�W����
	virtual void Damage(int attack);

	void CreateHeadLife();

	void CreateDeathEffect();

protected:
	virtual void Init();
};