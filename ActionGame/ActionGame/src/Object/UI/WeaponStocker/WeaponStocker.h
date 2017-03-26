// 所持武器の表示用Sprite
// 2017. 3.25	プログラム作成
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

	// 武器の種類を設定
	void SetWeaponType(_eWeaponType type);

private:
	virtual void Update();
	virtual void Draw();

	// 剣チップ作成
	void CreateSwordTip();
	// 杖チップ作成
	void CreateStickTip();

	// チップの座標を再設定
	void ReSetTipPos(int WeaponType);
};