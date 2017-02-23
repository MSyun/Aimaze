// 敵
// 2017. 1.31	プログラム作成
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

	// ダメージ処理
	virtual void Damage(int attack);

	void CreateHeadLife();

	void CreateDeathEffect();

protected:
	virtual void Init();
};