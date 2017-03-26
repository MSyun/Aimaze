// 武器クラス
// 2017. 1.24	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../../GameFramework/Obj/3D/Obj3D.h"
#include	"../Character/CharacterStatus/CharacterStatus.h"

typedef enum {
	WEAPON_SWORD = 0,
	WEAPON_STICK,

	WEAPON_MAX
} _eWeaponType;


class Character;

class Weapon	:	public	Obj3D {
private:
	CharacterStatus	m_Status;

	Character*	m_pOwner;	// 所有者

	_eWeaponType	m_WeaponType;	// 武器タイプ

public:
	Weapon();
	virtual ~Weapon();

	CharacterStatus& GetStatus() { return m_Status; }

	_eWeaponType GetWeaponType() { return m_WeaponType; }
	void SetWeaponType(_eWeaponType type) { m_WeaponType = type; }

	// 持ち主取得
	Character* GetOwner() { return m_pOwner; }
	// 持ち主設定
	void SetOwner(Character* obj);

protected:
	virtual void LateUpdate();
};