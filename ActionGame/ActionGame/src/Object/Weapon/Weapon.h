// 武器クラス
// 2017. 1.24	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../../GameFramework/Obj/3D/Obj3D.h"
#include	"../Character/CharacterStatus/CharacterStatus.h"


class Character;

class Weapon	:	public	Obj3D {
private:
	CharacterStatus	m_Status;

	Character*	m_pOwner;	// 所有者

	unsigned char	m_WeaponType;	// 武器タイプ

public:
	Weapon();
	virtual ~Weapon();

	CharacterStatus& GetStatus() { return m_Status; }

	void SetWeaponType(unsigned char type) { m_WeaponType = type; }

	// 持ち主取得
	Character* GetOwner() { return m_pOwner; }
	// 持ち主設定
	void SetOwner(Character* obj);

protected:
	virtual void LateUpdate();
};