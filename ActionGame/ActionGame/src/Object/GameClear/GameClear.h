// �Q�[���N���A�\�N���X
// 2017. 2. 13	�v���O�����쐬
// author		SyunMizuno


#pragma once


#include	"../../GameFramework/Sprite/Moving/MovingSprite.h"
#include	"../../GameFramework/Sprite/Animation/SpriteAnimation.h"



class GameClear	:	public	SpriteAnimation {
private:
	MovingSprite*	m_pFont;

public:
	GameClear();
	virtual ~GameClear();

private:
	virtual void Input();
};