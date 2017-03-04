// ドラゴンの炎クラス
// 2017. 2. 7	プログラム作成
// Author		SyunMizuno

#pragma once


#include	"../../AttackCollisionSpace/AttackCollisionSpace.h"
#include	"../../GameFramework/framework/Particle/Particle.h"


class DragonBreath	:	public	AttackCollisionSpace {
private:
	// 移動方向
	Vector3		m_vMove;
	// 軌跡
	Particle*	m_pLocus;
	// 煙
	Particle*	m_pSmoke;
	bool		m_bSmoke;

public:
	DragonBreath();
	virtual ~DragonBreath();

	// 射出
	void Shot(Transform* shooter, float speed);

	virtual void OnCollisionStay(Obj3D* obj);

protected:
	// 更新
	virtual void Update();
	// 軌跡作成
	void CreateLocus();
	// 煙作成
	void CreateSmoke();
	// 爆発
	void Burn();
};