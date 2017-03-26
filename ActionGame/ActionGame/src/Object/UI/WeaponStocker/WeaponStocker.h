// ��������̕\���pSprite
// 2017. 3.25	�v���O�����쐬
// author		SyunMizuno


#pragma once



#include	"../../../GameFramework/Sprite/Moving/MovingSprite.h"
#include	"../../Weapon/Weapon.h"


class WeaponStocker	:	public	MovingSprite {
private:
	vector<Sprite*> m_WeaponTip;
	int m_nCurrentWeapon;

public:
	WeaponStocker();
	virtual ~WeaponStocker();

	// ����̎�ނ�ݒ�
	void SetWeaponType(_eWeaponType type);

private:
	virtual void Update();
	virtual void Draw();

	// ���`�b�v�쐬
	void CreateSwordTip();
	// ��`�b�v�쐬
	void CreateStickTip();

	// �`�b�v�̍��W���Đݒ�
	void ReSetTipPos(int WeaponType);
};