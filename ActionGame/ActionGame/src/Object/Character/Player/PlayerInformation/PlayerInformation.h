// プレイヤー情報
// 2017. 2. 9	プログラム作成
// Author		SyunMizuno


#pragma once


#include	"../../../../GameFramework/Sprite/Animation/SpriteAnimation.h"
#include	"../../../Gauge/2D/Gauge2.h"


class PlayerInformation	:	public	Sprite {
private:
	vector<SpriteAnimation*>	m_aHitPoint;
	Gauge2*			m_pGauge;

	int	m_nMaxHp;
	int	m_nCurHp;

public:
	PlayerInformation();
	virtual ~PlayerInformation();

	void SetMaxHp(int max);
	void SetCurHp(int hp);



protected:
	virtual void Init();
};