// ����N���X
// 2017. 1.24	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"../../GameFramework/Obj/3D/Obj3D.h"
#include	"../Character/CharacterStatus/CharacterStatus.h"


class Character;

class Weapon	:	public	Obj3D {
private:
	CharacterStatus	m_Status;

	Character*	m_pOwner;	// ���L��

	unsigned char	m_WeaponType;	// ����^�C�v

public:
	Weapon();
	virtual ~Weapon();

	CharacterStatus& GetStatus() { return m_Status; }

	void SetWeaponType(unsigned char type) { m_WeaponType = type; }

	// ������擾
	Character* GetOwner() { return m_pOwner; }
	// ������ݒ�
	void SetOwner(Character* obj);

protected:
	virtual void LateUpdate();
};