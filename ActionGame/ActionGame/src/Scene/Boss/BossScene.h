// �{�X�V�[��
// 2017. 2. 6	�v���O�����쐬
// author		SyunMizuno


#pragma once


#include	"../../GameFramework/framework/Scene/SceneBase.h"
#include	"../../ItemBox/ItemBoxManager.h"


class BossScene	: public	SceneBase {
private:
	ItemBoxManager*		m_pItemBoxManager;

public:
#pragma region method

	BossScene();
	virtual ~BossScene();

	virtual void Update();
	virtual void Draw();
	virtual bool ReStart();

#pragma endregion

protected:
#pragma region conceal method

	virtual bool Init();
	virtual void Release();

	void LoadRate(int rate);
	void EnemyCreate();

#pragma endregion
};