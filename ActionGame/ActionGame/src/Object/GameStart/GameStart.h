// �Q�[���X�^�[�g�X�v���C�g
// 2017. 2. 7	�v���O�����쐬
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