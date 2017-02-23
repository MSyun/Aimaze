// アクションゲーム用DirectX9
// 2017. 1. 1	プログラム作成
// author		SyunMizuno


#pragma once


#include	"../GameFramework/DirectX/GameDirectX9Base.h"



class ActionGameDX9	:	public	GameDirectX9Base {
protected:
#pragma region variable



#pragma endregion

public:
#pragma region method

	ActionGameDX9(Application* pApp);
	virtual ~ActionGameDX9();

#pragma endregion

protected:
#pragma region conceal method

	virtual HRESULT InitGame();
	virtual HRESULT ReleaseGame();
	virtual HRESULT InvalidateDeviceObjectsGame();
	virtual HRESULT RestoreDeviceObjectsGame();

#pragma endregion
};