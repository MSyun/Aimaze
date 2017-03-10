// キャラクタークラス
// 2017. 1. 3	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../../GameFramework/Obj/3D/Obj3D.h"
#include	"CharacterStatus/CharacterStatus.h"

#include	"../Weapon/Weapon.h"


class Character : public Obj3D {
private:
	CharacterStatus	m_Status;	// ステータス
	Weapon*		m_pWeapon;

	SkinMesh*	m_pStageMesh;	// ステージのメッシュ

public:
	Character();
	virtual ~Character();

	CharacterStatus& GetStatus() { return m_Status; }

	Weapon* GetWeapon() { return m_pWeapon; }
	virtual void SetWeapon(Weapon* weapon);

	virtual void Damage(int attack) { m_Status.Damage(attack); };

protected:
#pragma region conceal method

	// 初期化
	virtual void Init();

	// OBBよりレイ用座標解析
	Point3 AnalyzeRayPos(
		const Point3& pos,
		const Obb& obb,
		bool x,
		bool y,
		bool z);

	// 壁判定確認
	// return. 移動量
	Vector3 WallCheck(Vector3& speed);

	// 入力
	virtual void Input();

	// 移動の入力
	virtual void MoveInput(const string cameraName);

	// 攻撃の入力
	virtual void AttackInput() {}

#pragma endregion
};