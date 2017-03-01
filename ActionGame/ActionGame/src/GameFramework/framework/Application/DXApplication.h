//////////////////////////////////////////////////////////////
// 2016. 2. 7	プログラム作成
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#pragma once

#include	"Application.h"
#include	"../DirectX/DXBase.h"


class	DXApplication : public	Application {
public:
#pragma region method

	DXApplication(
		int width = 600,
		int height = 480,
		bool bwnd = true);
	virtual ~DXApplication();

	// DirectXのオブジェクトを設定
	void RegistDXObj(IDXBase* _obj) { m_pDXObj = _obj; }

	virtual LRESULT LocalWindowProc(
		HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

#pragma endregion

protected:
#pragma region variable

	IDXBase*	m_pDXObj;

#pragma endregion

#pragma region conceal method

	virtual void PreMainWnd();
	virtual bool Initialize();
	virtual void Loop();
	virtual void ReleaseApp();

#pragma endregion
};