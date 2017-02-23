// コンフィグシーン
// 2017. 2. 4	プログラム作成
// author		SyunMizuno


#pragma once


#include	"../../GameFramework/framework/Scene/SceneBase.h"



class ConfigScene	:	public	SceneBase {
public:
#pragma region method

	ConfigScene();
	virtual ~ConfigScene();

	virtual void Update();
	virtual void Draw();
	virtual bool ReStart() { return true; };

#pragma endregion

private:
#pragma region conceal method

	virtual bool Init();
	virtual void Release();

#pragma endregion
};