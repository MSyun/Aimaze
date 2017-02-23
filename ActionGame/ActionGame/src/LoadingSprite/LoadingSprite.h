// ウェーブスプライト
// 2017. 2. 4	プログラム作成
// author		SyunMizuno


#pragma once


#include	"../GameFramework/Sprite/Moving/MovingSprite.h"
#include	<vector>
using namespace std;


class LoadingSprite {
private:
	vector<MovingSprite*>	m_aSprite;

public:
	LoadingSprite();
	virtual ~LoadingSprite();

	void Update();
	void Draw();
};