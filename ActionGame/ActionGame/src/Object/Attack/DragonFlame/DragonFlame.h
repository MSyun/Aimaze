// �h���S���̉��N���X
// 2017. 2. 7	�v���O�����쐬
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