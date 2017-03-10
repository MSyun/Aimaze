// �����蔻��͈̓N���X
// 2017. 1.31	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"../../Object/Character/Charcter.h"
#include	"../SlashEffect/SlashEffect.h"


class AttackCollisionSpace : public	Obj3D {
protected:
	int		m_nPower;

	//----- �G�̃��X�g
	vector<Obj3D*>		m_aHit;

	SlashEffect*	m_pEffect;

public:
	AttackCollisionSpace();
	virtual ~AttackCollisionSpace();

	void SetPower(int pow) { m_nPower = pow; }
	int GetPower() { return m_nPower; }

	void Reset();

	virtual void OnCollisionStay(Obj3D* obj);

protected:

	virtual void Init();
};