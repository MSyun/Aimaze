// 魔法攻撃クラス
// 2017. 1.31	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../../GameFramework/framework/Particle/Particle.h"
#include	"../../AttackCollisionSpace/AttackCollisionSpace.h"



class AttackMagic	:	public	AttackCollisionSpace {
protected:
	float		m_fSpeed;
	float		m_fTime;
	float		m_fDefaultTime;
	Particle*	m_pParticle;

public:
	AttackMagic();
	virtual ~AttackMagic();

	void SetSpeed(float speed) { m_fSpeed = speed; }
	void SetDefaultTime(float time) { m_fDefaultTime = time; }

	void Shot();

protected:
	virtual void Update();
};