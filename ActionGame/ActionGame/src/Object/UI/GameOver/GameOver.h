// ゲームオーバークラス
// 2017. 2. 9	プログラム作成
// author		SyunMizuno


#pragma once


#include	"../../../GameFramework/Sprite/Moving/MovingSprite.h"
#include	"../../../GameFramework/Sprite/FadeColor/FadeColorSprite.h"



class GameOver	:	public	MovingSprite {
private:
	FadeColorSprite*	m_pBack;

public:
	GameOver();
	virtual ~GameOver();

private:
	virtual void Input();
};