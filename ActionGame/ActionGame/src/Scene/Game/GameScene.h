// ゲームシーン
// 2017. 1. 1	プログラム作成
// author		SyunMizuno


#pragma once


#include	"../../GameFramework/framework/Scene/SceneBase.h"
#include	"../../Object/ItemBox/ItemBoxManager.h"

class GameScene	:	public	SceneBase {
private:
	ItemBoxManager*		m_pItemBoxManager;

public:
#pragma region method

	GameScene();
	virtual ~GameScene();

	virtual void Update();
	virtual void Draw();
	virtual bool ReStart();

#pragma endregion

protected:
#pragma region conceal method

	virtual bool Init();
	virtual void Release();

	void LoadRate(int num);
	void EnemyCreate();
	void SpriteCreate();
	void WeaponCreate();

#pragma endregion
};