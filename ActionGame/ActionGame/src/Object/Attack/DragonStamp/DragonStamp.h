// �h���S���̓��݂Ԃ��N���X
// 2017. 2.14	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"../../../GameFramework/framework/Particle/Particle.h"
#include	"../../../Object/AttackCollisionSpace/AttackCollisionSpace.h"



class DragonStamp	:	public	AttackCollisionSpace {
private:
	// ��
	Particle*	m_pSmoke;

public:
	DragonStamp();
	virtual ~DragonStamp();

	void Stamp();

private:
	// ���쐬
	void CreateSmoke();
};