// ドラゴンの踏みつぶしクラス
// 2017. 2.14	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../../../GameFramework/framework/Particle/Particle.h"
#include	"../../../Object/AttackCollisionSpace/AttackCollisionSpace.h"



class DragonStamp	:	public	AttackCollisionSpace {
private:
	// 煙
	Particle*	m_pSmoke;

public:
	DragonStamp();
	virtual ~DragonStamp();

	void Stamp();

private:
	// 煙作成
	void CreateSmoke();
};