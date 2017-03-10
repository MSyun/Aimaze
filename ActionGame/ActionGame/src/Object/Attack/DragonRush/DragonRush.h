// ドラゴンの突っ込みクラス
// 2017. 2. 9	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../../../GameFramework/framework/Particle/Particle.h"
#include	"../../../Object/AttackCollisionSpace/AttackCollisionSpace.h"
#include	"../../../GameFramework/Locus/Locus.h"


class DragonRush	:	public	AttackCollisionSpace {
private:
	// 煙
	Particle*	m_pSmoke;
	// 爪の跡
	Locus*		m_pLocus;


public:
	DragonRush();
	virtual ~DragonRush();

	void Record();
	void End();

protected:
	// 煙作成
	void CreateSmoke();
	// 軌跡作成
	void CreateLocus();
	// 更新
	virtual void Update();
	// 描画
	virtual void LateDraw();
};