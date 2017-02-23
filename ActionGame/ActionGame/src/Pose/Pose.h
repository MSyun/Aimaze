// ポーズ
// 2017. 2. 10	プログラム作成
// author		SyunMizuno

#pragma once


#include	"../GameFramework/Obj/2D/Sprite.h"
#include	"../GameFramework/Sprite/FadeColor/FadeColorSprite.h"
#include	"../GameFramework/Obj/3D/Obj3D.h"


class Pose	:	public	Sprite {
protected:
	bool	m_bOpen;
	vector<Sprite*>		m_aSprite;
	FadeColorSprite*	m_pSelector;
	vector<Sprite*>		m_aMode;
	int	m_nSelect;

	Obj3D*	m_pPlayer;

	void Open();
	void Close();

	bool ModeSelect();
	void Selection();

public:
	Pose();
	virtual ~Pose();

private:
	virtual void Init();
	virtual void Update();
};