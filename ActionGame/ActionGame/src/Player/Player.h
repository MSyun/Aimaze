// プレイヤー
// 2017. 1.22	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../Character/Charcter.h"
#include	"../GameFramework/Locus/Locus.h"
#include	"../AttackCollisionSpace/AttackCollisionSpace.h"
#include	"../PlayerInformation/PlayerInformation.h"


class Player	:	public	Character {
private:
#pragma region struct

	struct _Attack {
		Locus					m_Locus;
		AttackCollisionSpace	m_HitSpace;
	};

#pragma endregion

#pragma region variable

	PlayerInformation*	m_pInfo;

	_Attack*	m_pAttack;
	bool		m_bAttack;
	bool		m_bHit;

#pragma endregion

public:
#pragma region method

	Player();
	virtual ~Player();

	// 当たっている間
	virtual void OnCollisionStay(Obj3D* obj);

	virtual void Damage(int attack);

#pragma endregion

protected:
#pragma region conceal method

	virtual void Init();
	virtual void LateUpdate();
	virtual void LateDraw();

	void Walk(float speed);
	virtual void MoveInput(const string cameraName);
	virtual void AttackInput();

#pragma endregion
};