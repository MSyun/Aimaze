// ゲームスタートスプライト
// 2017. 2. 7	プログラム作成
// author		SyunMizuno



#pragma once


#include	"../../GameFramework/Sprite/Moving/MovingSprite.h"


class GameStart	:	public	MovingSprite {
private:
	float	m_fTime;

public:
	GameStart();
	virtual ~GameStart();

private:
	virtual void Update();
};