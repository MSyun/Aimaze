// ローディングシーンクラス
// 2017. 2. 4	プログラム作成
// Author		SyunMizuno


#pragma once



#include	"../../GameFramework/framework/Scene/SceneBase.h"
#include	"../../Gauge/Gauge2.h"
#include	<vector>
#include	"../../LoadingSprite/LoadingSprite.h"



class LoadScene	:	public	SceneBase {
private:
	Gauge2* m_pGauge;
	vector<Sprite*>	m_aSprite;
	LoadingSprite*	m_pLoadSprite;

public:
	LoadScene();
	virtual ~LoadScene();

	virtual void Update();
	virtual void Draw();
	virtual bool ReStart();

private:
	virtual bool Init();
	virtual void Release();
};