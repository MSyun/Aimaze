// タイトルシーン
// 2017. 1. 1	プログラム作成
// author		SyunMizuno


#pragma once


#include	"../../GameFramework/framework/Scene/SceneBase.h"


class TitleLogoScene	:	public	SceneBase {
private:
#pragma region variable

	bool	m_bChange;

	float	m_fTime;

#pragma endregion

public:
#pragma region method

	TitleLogoScene();
	virtual ~TitleLogoScene();

	virtual void Update();
	virtual void Draw();
	virtual bool ReStart();

#pragma endregion

protected:
#pragma region conceal method

	virtual bool Init();
	virtual void Release();

#pragma endregion
};