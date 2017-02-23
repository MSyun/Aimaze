// ミッションクラス
// 2017. 2. 7	プログラム作成
// author		SyunMizuno


#pragma once


#include	"../GameFramework/Sprite/Scaling/ScalingSprite.h"
#include	"../GameFramework/Sprite/FadeColor/FadeColorSprite.h"


class Mission	:	public	ScalingSprite {
private:
	FadeColorSprite* m_pButton;

public:
	Mission(
		Vector3 begin = Vector3(0.0f, 0.0f, 0.0f),
		Vector3 end = Vector3(1.0f, 1.0f, 1.0f),
		float time = 2.0f,
		bool reverse = false,
		bool awake = false);
	virtual ~Mission();

protected:
	virtual void Input();
};