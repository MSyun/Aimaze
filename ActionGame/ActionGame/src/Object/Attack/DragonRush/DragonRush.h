// �h���S���̓˂����݃N���X
// 2017. 2. 9	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"../../../GameFramework/framework/Particle/Particle.h"
#include	"../../../Object/AttackCollisionSpace/AttackCollisionSpace.h"
#include	"../../../GameFramework/Locus/Locus.h"


class DragonRush	:	public	AttackCollisionSpace {
private:
	// ��
	Particle*	m_pSmoke;
	// �܂̐�
	Locus*		m_pLocus;


public:
	DragonRush();
	virtual ~DragonRush();

	void Record();
	void End();

protected:
	// ���쐬
	void CreateSmoke();
	// �O�Ս쐬
	void CreateLocus();
	// �X�V
	virtual void Update();
	// �`��
	virtual void LateDraw();
};