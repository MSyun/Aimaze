// �L�����N�^�[�N���X
// 2017. 1. 3	�v���O�����쐬
// Author		SyunMizuno


#pragma once


#include	"../../GameFramework/Obj/3D/Obj3D.h"
#include	"CharacterStatus/CharacterStatus.h"

#include	"../Weapon/Weapon.h"


class Character : public Obj3D {
private:
	CharacterStatus	m_Status;	// �X�e�[�^�X
	Weapon*		m_pWeapon;

	SkinMesh*	m_pStageMesh;	// �X�e�[�W�̃��b�V��

public:
	Character();
	virtual ~Character();

	CharacterStatus& GetStatus() { return m_Status; }

	Weapon* GetWeapon() { return m_pWeapon; }
	virtual void SetWeapon(Weapon* weapon);

	virtual void Damage(int attack) { m_Status.Damage(attack); };

protected:
#pragma region conceal method

	// ������
	virtual void Init();

	// OBB��背�C�p���W���
	Point3 AnalyzeRayPos(
		const Point3& pos,
		const Obb& obb,
		bool x,
		bool y,
		bool z);

	// �ǔ���m�F
	// return. �ړ���
	Vector3 WallCheck(Vector3& speed);

	// ����
	virtual void Input();

	// �ړ��̓���
	virtual void MoveInput(const string cameraName);

	// �U���̓���
	virtual void AttackInput() {}

#pragma endregion
};