// ドラゴンの炎クラス
// 2017. 2. 7	プログラム作成
// Author		SyunMizuno

#pragma once


#include	"../../../GameFramework/framework/Particle/Particle.h"
#include	"../../../Object/AttackCollisionSpace/AttackCollisionSpace.h"



class DragonFlame	:	public	AttackCollisionSpace {
protected:
	Particle*	m_pParticle;

public:
	DragonFlame();
	virtual ~DragonFlame();
};