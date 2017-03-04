// �h���S���̉��N���X
// 2017. 2. 7	�v���O�����쐬
// Author		SyunMizuno

#pragma once


#include	"../../AttackCollisionSpace/AttackCollisionSpace.h"
#include	"../../GameFramework/framework/Particle/Particle.h"


class DragonBreath	:	public	AttackCollisionSpace {
private:
	// �ړ�����
	Vector3		m_vMove;
	// �O��
	Particle*	m_pLocus;
	// ��
	Particle*	m_pSmoke;
	bool		m_bSmoke;

public:
	DragonBreath();
	virtual ~DragonBreath();

	// �ˏo
	void Shot(Transform* shooter, float speed);

	virtual void OnCollisionStay(Obj3D* obj);

protected:
	// �X�V
	virtual void Update();
	// �O�Ս쐬
	void CreateLocus();
	// ���쐬
	void CreateSmoke();
	// ����
	void Burn();
};